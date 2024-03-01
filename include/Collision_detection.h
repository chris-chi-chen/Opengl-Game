//原本要用但太過不精準
#ifndef collision_CLASS_H
#define collision_CLASS_H

class OctreeNode {
public:
    glm::vec3 center; // 節點中心
    float size;       // 節點大小
    OctreeNode* children[8]; // 八個子節點

    // 存儲點的向量
    std::vector<glm::vec3> points;

    // 構造函數
    OctreeNode(glm::vec3 center, float size) : center(center), size(size) {
        for (int i = 0; i < 8; ++i) {
            children[i] = nullptr;
        }
    }

    // 析構函數
    ~OctreeNode() {
        for (int i = 0; i < 8; ++i) {
            delete children[i];
        }
    }

    // 確定點是否在節點範圍內
    bool containsPoint(const glm::vec3& point) {
        return (point.x >= center.x - size / 2 && point.x <= center.x + size / 2) &&
               (point.y >= center.y - size / 2 && point.y <= center.y + size / 2) &&
               (point.z >= center.z - size / 2 && point.z <= center.z + size / 2);
    }

    // 添加點
    void insert(const glm::vec3& point) {
        // 如果節點包含點，則處理它
        if (containsPoint(point)) {
            if (points.size() < MAX_POINTS || size <= MIN_SIZE) {
                points.push_back(point);
            } else {
                // 如果達到最大點數或最小大小，則分割節點
                if (children[0] == nullptr) {
                    subdivide();
                }
                for (int i = 0; i < 8; ++i) {
                    children[i]->insert(point);
                }
            }
        }
    }
    bool checkCollision(const glm::vec3& point, float threshold) {
        if (!containsPoint(point)) {
            return false;
        }

        for (const auto& existingPoint : points) {
            if (glm::distance(glm::vec3(point.x,0,point.z), glm::vec3(existingPoint.x, 0, existingPoint.z)) < threshold) {
                return true; // 發現碰撞
            }
        }

        for (int i = 0; i < 8; ++i) {
            if (children[i] != nullptr && children[i]->checkCollision(point, threshold)) {
                return true; // 在子節點中發現碰撞
            }
        }

        return false; // 未發現碰撞
    }
private:
    void subdivide() {
        float newSize = size / 2;
        glm::vec3 newCenter;

        for (int i = 0; i < 8; ++i) {
            newCenter.x = center.x + newSize * (i & 1 ? 0.5f : -0.5f);
            newCenter.y = center.y + newSize * (i & 2 ? 0.5f : -0.5f);
            newCenter.z = center.z + newSize * (i & 4 ? 0.5f : -0.5f);
            children[i] = new OctreeNode(newCenter, newSize);
        }

        // 重新分配現有點到新的子節點
        for (const glm::vec3& point : points) {
            for (int i = 0; i < 8; ++i) {
                if (children[i]->containsPoint(point)) {
                    children[i]->insert(point);
                    break;
                }
            }
        }
        points.clear(); // 清除當前節點的點，因為它們已被分配到子節點
    }

    // 最大點數和最小節點大小常量
    static const int MAX_POINTS = 4;
    static const float MIN_SIZE;
};

const float OctreeNode::MIN_SIZE = 0.01f;

// Octree 類
class Octree {
public:
    OctreeNode* root;

    // 構造函數
    Octree(glm::vec3 center, float size) {
        root = new OctreeNode(center, size);
    }

    // 析構函數
    ~Octree() {
        delete root;
    }

    // 插入點到 Octree
    void insert(const glm::vec3& point) {
        root->insert(point);
    }
    bool checkCollision(const glm::vec3& point, float threshold) {
        if (root == nullptr) {
            return false;
        }
        return root->checkCollision(point, threshold);
    }
};

#endif // COLLISION_CLASS_H
