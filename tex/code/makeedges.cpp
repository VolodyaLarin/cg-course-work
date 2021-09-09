static std::vector<QpEdge> makeEdges(const std::vector<std::shared_ptr<ICoord>> &points, QRect borders) {
    // Граница экрана
    QVector<QPointF> borders_list = {
            QPointF(0,0),
            QPointF(0, borders.height() - 1),
            QPointF(borders.width() - 1, borders.height() - 1),
            QPointF(borders.width() - 1, 0)
    };
    // Нормали ребер экрана
    QVector<QPointF> normals_list = {
            QPointF(-1, 0),
            QPointF(0, 1),
            QPointF(1, 0),
            QPointF(0, -1)
    };

    // Отсечение полигона плоскостью экрана
    auto cutted_figure = MathUtilits::cut_figure(points, borders_list, normals_list); 


    std::vector<QpEdge> edges = {};
    unsigned long count = cutted_figure.size();
    edges.reserve(count);

    QpEdge temp;
    for (int i = 0; i < count; ++i) {
        auto p1 = cutted_figure[i], p2 = cutted_figure[(i + 1) % count];

        temp.begin = {int(p1.x()), int(p1.y())};
        temp.end = {int(p2.x()), int(p2.y())};

        if (temp.begin.y() != temp.end.y())
        {
            if (temp.begin.y() > temp.end.y()) {
                auto left = temp.begin;
                temp.begin = temp.end;
                temp.end = left;
            }

            temp.k = (temp.begin.x() + 0.0 - temp.end.x()) / (temp.begin.y() - temp.end.y());

            edges.emplace_back(temp);
        }
    }

    std::sort(edges.begin(), edges.end(), [](const QpEdge& left, const QpEdge& right){
        // Сортировка массива ребер
        if (left.begin.y() == right.begin.y()) {
            if (left.begin.x() == right.begin.x()) {
                if (left.end.x() == right.end.x()) {
                    return left.end.y() < right.end.y();
                }
                return left.end.x() < right.end.x();
            }
            return left.begin.x() < right.begin.x();
        }
        return left.begin.y() < right.begin.y();
    });

    return edges;
}