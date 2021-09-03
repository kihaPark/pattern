#include <iostream>
#include <vector>
#include <array>


// tested x64, c++17

using namespace std;

class Mesh;


class Matrix
{
public:
    virtual ~Matrix() = default;

    static Matrix origin() {
        return Matrix();
    }

    Matrix transform(Matrix& other) {
        return other;
    }

public:
    static const int MAX_ROW = 4;
    static const int MAX_COL = 4;
    static constexpr int MAX_SIZE = MAX_ROW * MAX_COL;

    array<int, MAX_SIZE> elements = { 0, };
};


class GraphNode
{
public:
    GraphNode(int parentId) { create(parentId); }
    virtual ~GraphNode() = default;

    void create(int parentId);
    GraphNode& addChild();
    void setMatrix(Matrix& localPos);
    void setMesh(Mesh* mesh);
    void update(Matrix& worldPos, bool dirty);
    void render();
    void viewStatus();

private:
    int nodeId;

    int parentId;
    vector<GraphNode> children;

    bool dirty;
    Matrix world;
    Matrix local;

    Mesh* mesh;
};

void GraphNode::create(int parentId)
{
    static int genId = 1;
    nodeId = genId;
    ++genId;

    this->parentId = parentId;

    dirty = false;
    world.elements.fill(0);
    local.elements.fill(0);

    mesh = nullptr;
}

GraphNode& GraphNode::addChild()
{
    children.push_back(GraphNode(nodeId));
    return children.back();
}

void GraphNode::setMatrix(Matrix& localPos)
{
    local = localPos;
    dirty = true;

    cout << "dirty on, node id: " << nodeId << endl;
}

void GraphNode::setMesh(Mesh* mesh)
{
    this->mesh = mesh;
}

void GraphNode::update(Matrix& worldPos, bool isDirty)
{
    bool dirtyOn = dirty | isDirty;

    if (dirtyOn) {
        world = local.transform(worldPos);
        dirty = false;

        cout << "update node id: " << nodeId << endl;
    }

    for (auto& child : children)
        child.update(world, dirtyOn);
}

void GraphNode::render()
{
    cout << "render node id: " << nodeId << endl;

    for (auto& child : children)
        child.render();
}

void GraphNode::viewStatus()
{
    cout << "(" << parentId << ", " << nodeId << ") : dirty on " << dirty << endl;

    for (auto& child : children)
        child.viewStatus();
}


int main()
{
    Matrix move;

    GraphNode scene{ 0 };
    scene.addChild();

    GraphNode& child1 = scene.addChild();
    child1.setMatrix(move);
    child1.addChild();
    child1.addChild();
    child1.addChild();

    scene.addChild().addChild();

    scene.viewStatus();

    cout << endl << "update" << endl;
    scene.update(move, false);
    scene.render();

    cout << endl << "update all" << endl;
    scene.update(move, true);
    scene.render();

    return 0;
}

