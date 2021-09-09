void QpDrawer::drawPolygon(const Polygon &polygon, const IColor &color) {
    // Поиск нормали полигона с учетом преобразований камеры и объекта
    auto normal = _objectTrans->apply(polygon.getNormal());
    auto normal_begin = _objectTrans->apply(*polygon.getNormal().clone()->mul(0));
    normal->sub(*normal_begin);
    normal->normalize();

    auto gr0 = [](float x) {
        return x > 0 ? x : 1;
    };
    float intens = 0.2f + 0.8f - gr0(std::acos(
        normal->getX() * LightVec[0] +
        normal->getY() * LightVec[1] +
        normal->getZ() * LightVec[2]
    ) / M_PI) * 0.8f;

    auto qColor = qRgb(
            color.getR() * 255 * intens,
            color.getG() * 255 * intens,
            color.getB() * 255 * intens
    ); // цвет полигона с учетом освещения

    std::vector<std::shared_ptr<ICoord>> points;
    
    for (auto &coord: polygon) {
        // Применение преобразований камера и модели к полигону
        std::shared_ptr<ICoord> new_coord = _cameraTrans->apply(*_objectTrans->apply(*coord));
        
        // Переход в экранную систему координат
        auto sizeK = _image.size().width();
        new_coord->mulled(sizeK);
        new_coord->setX(new_coord->getX() + _image.size().width() / 2.0f);
        new_coord->setY(-new_coord->getY() + _image.size().height() / 2.0f);

        points.push_back(new_coord);
    }

    auto surface = QpSurface::makeSurf(points);
    float kz = surface.a / surface.c;

    auto edges = QpEdge::makeEdges(points, _image.rect());

    if (edges.size() < 2) return;

    auto edgeL = edges[0];
    auto edgeR = edges[1];
    int last = 1;

    double xL = edgeL.begin.x();
    double xR = edgeR.begin.x();

    if (xL > xR) {
        std::swap(xL, xR);
        std::swap(edgeL, edgeR);
    }

    double yMin = edges.begin()->begin.y(); // первое ребро
    double yMax = edges.rbegin()->end.y(); // последнее ребро

    for (int y = yMin; y < yMax; ++y) {
        float z = findZ(surface, xL, y);
        for (int x = xL; x < xR; ++x) {
            if (z > 0 && z < _context->z->get(x, y)) {
                _context->z->set(x, y, z);
                _image.setPixel(x, y, qColor);
            }
            z += kz;
        }
        xL += edgeL.k;
        xR += edgeR.k;

        if (edgeL.end.y() <= y) {
            last++;
            edgeL = edges[last];
            xL = edgeL.begin.x();
            if (xL > xR) {
                std::swap(xL, xR);
                std::swap(edgeL, edgeR);
            }
        }
        if (edgeR.end.y() <= y) {
            last++;
            edgeR = edges[last];
            xR = edgeR.begin.x();
            if (xL > xR) {
                std::swap(xL, xR);
                std::swap(edgeL, edgeR);
            }
        }
    }
}