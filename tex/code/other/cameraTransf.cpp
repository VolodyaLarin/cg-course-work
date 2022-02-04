ITransformation::Ptr Camera::getTransformation(Time t) const {
    auto transformation = _transformationFactory->createPosition(
        *getPosition().get(t)->invert()
    );
    auto rotate = _transformationFactory->createRotation(
        *BaseSceneObject::getRotation().get(t)->invert()
    );
    transformation->join(*rotate);
    return transformation;
}