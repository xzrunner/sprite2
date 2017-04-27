# Flatten

## 目的

把数据组织成缓存友好的形式，降低cpu的开销，提高帧率，进而降低耗电和发热。

## 原理

在场景树中每个节点是单独分配内存的。由于内存不连续，在draw或者update时，缓存会经常失效。再从从内存中读，开销很大。

flatten能把节点展平，从一棵树的结构变成一块连续的数组。同时也减小了矩阵变换的开销。

去掉多余的层次结构。树的结构主要是为了方便编辑和使用，以及资源复用。比如，美术在制作资源时，为了方便会把一些节点打成一组，而且这个节点没有名字，游戏运行时也不会被操作，这样就多了一层。

## 实现

```
struct Quad
{
	int tex_id;
	sm::vec2 vertices[4], texcoords[4];
}; // Quad

struct Node
{
	const Sprite* spr;
	const Actor*  actor;
	S2_MAT mat;
	int idx;
}; // Node

std::vector<Quad> m_quads;
std::vector<Node> m_nodes;
```

用Quad表示一张图片，乘以每一层矩阵，计算出顶点坐标vertices。

Node表示不能被展开的节点，比如粒子。idx用来表示Node在Quad数组中的位置。