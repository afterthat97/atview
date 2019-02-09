#pragma once

#include <Scene.h>

class SceneTreeWidget: public QTreeWidget {
    Q_OBJECT

public:
    SceneTreeWidget(Scene* scene, QWidget* parent = 0);

    void setScene(Scene* scene);

public slots:
    void reload();

protected:
    void keyPressEvent(QKeyEvent *e) override;

signals:
    void itemSelected(QVariant item);
    void itemDeselected(QVariant item);

private:
    Scene* m_host;

private slots:
    void currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void gridlineAdded(Gridline* gridline);
    void lightAdded(AbstractLight* light);
    void modelAdded(Model* model);
    void hostDestroyed(QObject* host);
};


class BaseItem: public QObject, public QTreeWidgetItem {
    Q_OBJECT

public:
    BaseItem(QObject* host, QTreeWidgetItem* parent): QTreeWidgetItem(parent), QObject(0) {
        m_priority = 0;
        setData(0, Qt::UserRole, QVariant::fromValue(host));
        setText(0, host->objectName());
        connect(host, SIGNAL(destroyed(QObject*)), this, SLOT(hostDestroyed(QObject*)));
    }

    int priority() const {
        return m_priority;
    }

    void setPriority(int priority) {
        m_priority = priority;
    }

private:
    int m_priority;

private slots:
    void hostDestroyed(QObject*) {
        // Commit suicide
        delete this;
    }
};

#define CAMERA_PRIORITY 0
#define GRIDLINE_PRIORITY -1
#define AMBIENTLIGHT_PRIORITY -2
#define DIRECTIONALLIGHT_PRIORITY -3
#define POINTLIGHT_PRIORITY -4
#define SPOTLIGHT_PRIORITY -5
#define MESH_PRIORITY -6
#define MODEL_PRIORITY -7

class CameraItem: public BaseItem {
    Q_OBJECT

public:
    CameraItem(Camera* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(CAMERA_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/CameraIcon.png"));
    }

private:
    Camera* m_host;
};

class GridlineItem: public BaseItem {
    Q_OBJECT

public:
    GridlineItem(Gridline* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(GRIDLINE_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/GridlineIcon.png"));
    }

private:
    Gridline* m_host;
};

class AmbientLightItem: public BaseItem {
    Q_OBJECT

public:
    AmbientLightItem(AmbientLight* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(AMBIENTLIGHT_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/AmbientLightIcon.png"));
    }

private:
    AmbientLight* m_host;
};

class DirectionalLightItem: public BaseItem {
    Q_OBJECT

public:
    DirectionalLightItem(DirectionalLight* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(DIRECTIONALLIGHT_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/DirectionalLightIcon.png"));
    }

private:
    DirectionalLight* m_host;
};

class PointLightItem: public BaseItem {
    Q_OBJECT

public:
    PointLightItem(PointLight* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(POINTLIGHT_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/PointLightIcon.png"));
    }

private:
    PointLight* m_host;
};

class SpotLightItem: public BaseItem {
    Q_OBJECT

public:
    SpotLightItem(SpotLight* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(SPOTLIGHT_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/SpotLightIcon.png"));
    }

private:
    SpotLight* m_host;
};

class MaterialItem: public BaseItem {
    Q_OBJECT

public:
    MaterialItem(Material* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setIcon(0, QIcon(":/resources/icons/MaterialIcon.png"));
    }

private:
    Material* m_host;
};

class MeshItem: public BaseItem {
    Q_OBJECT

public:
    MeshItem(Mesh* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(MESH_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/MeshIcon.png"));
        if (m_host->material())
            new MaterialItem(m_host->material(), this);
    }

private:
    Mesh* m_host;
};

class ModelItem: public BaseItem {
    Q_OBJECT

public:
    ModelItem(Model* host, QTreeWidgetItem* parent): BaseItem(host, parent) {
        m_host = host;
        setPriority(MODEL_PRIORITY);
        setIcon(0, QIcon(":/resources/icons/ModelIcon.png"));
        for (int i = 0; i < m_host->childMeshes().size(); i++)
            new MeshItem(m_host->childMeshes()[i], this);
        for (int i = 0; i < m_host->childModels().size(); i++)
            new ModelItem(m_host->childModels()[i], this);
    }

private:
    Model* m_host;
};
