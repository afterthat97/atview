#pragma once

#include <Core/Common.h>

class Mesh;

class Model: public QObject {
    Q_OBJECT

public:
    Model(QObject * parent = 0);
    Model(const Model& model);
    ~Model();

    bool addChildMesh(Mesh* mesh);
    bool addChildModel(Model* model);

    bool removeChildMesh(QObject* mesh, bool recursive);
    bool removeChildModel(QObject* model, bool recursive);

    void translate(QVector3D delta);
    void rotate(QQuaternion rotation);
    void rotate(QVector3D rotation);
    void scale(QVector3D scaling);

    void dumpObjectInfo(int level = 0);
    void dumpObjectTree(int level = 0);

    QVector3D centerOfMass() const;
    float mass() const;

    bool visible() const;
    QVector3D localPosition() const;
    QVector3D globalPosition() const;
    QVector3D localRotation() const;
    QVector3D localScaling() const;
    QMatrix4x4 localModelMatrix() const;
    QMatrix4x4 globalModelMatrix() const;
    const QVector<Mesh*> & childMeshes() const;
    const QVector<Model*> & childModels() const;

public slots:
    void resetTransformation();
    void resetChildrenTransformation();
    void setVisible(bool visible);
    void setPosition(QVector3D position);
    void setRotation(QQuaternion rotation);
    void setRotation(QVector3D rotation);
    void setScaling(QVector3D scaling);

signals:
    void visibleChanged(bool visible);
    void positionChanged(QVector3D position);
    void rotationChanged(QVector3D rotation);
    void scalingChanged(QVector3D scaling);
    void childMeshAdded(Mesh* mesh);
    void childMeshRemoved(QObject* object);
    void childModelAdded(Model* model);
    void childModelRemoved(QObject* object);
    void childrenChanged();

protected:
    void childEvent(QChildEvent *event) override;

private:
    bool m_visible;
    QVector3D m_position, m_rotation, m_scaling;

    QVector<Mesh*> m_childMeshes;
    QVector<Model*> m_childModels;
};