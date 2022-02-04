unique_ptr<ITransformation> RenderVisitor::_createObjectTransformation(
const ISceneObject &obj, Time now)  {
    auto transformation = _transformationFactory->create();
    transformation->join(
        *_transformationFactory->createRotation(
            *obj.getRotation().get(now)
        )
    ).join(
        *_transformationFactory->createScale(
            *obj.getSize().get(now)
        )
    ).join(
        *_transformationFactory->createPosition(
            *obj.getPosition().get(now)
        )
    );
    return transformation;
}