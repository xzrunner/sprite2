# FAQ

## 为什么要分成三层结构来表示场景图？

只用一层结构构成一棵树，会有很多重复。比如不同Node中会用到的mesh的顶点信息、粒子发射器的参数、动画结构等，这些是不会改变的部分，Node实例化出来后会共享这部分内容。如果每个Node这些信息都创建一份的话，不仅会占用更多的内存，还会使信息冗余，不能只改一个地方就使同一类Node受到影响。

<a id="twolayer"></a>用两层结构，分离出不可变的部分和可变的部分，SNode（static）和DNode（dynamic）。DNode构成一棵树，每个DNode会有指针指向SNode。SNode只存不变的信息，相互之间没有联系。这个结构的问题是DNode会有重复，同一个parent-children的结构可能会用在多个地方，比如一个场景有10棵相同的树，一棵树上有10根相同的树枝，一根树枝上有10片相同的叶子，这样叶子DNode就有10×10×10=1000个。

sprite2里的Symbol和Sprite对应SNode和DNode，但又不完全相同。<a id="spr_sym"></a>Symbol表示的是Sprite的组成结构，Sprite是Symbol实例化出来的，每一个Symbol拥有1到n个Sprite，每一个Sprite只有1个Symbol。即树的结构是由Symbol和Sprite一起来表达，不是只由DNode来表达。对于上面那个例子，Sprite就只需要10+10+10=30个，而不是1000个。场景是1个Symbol，这个Symbol里有10个树的Sprite，每个树的Symbol有10个树枝的Sprite，每个树枝的Symbol有10个树叶的Sprite。

但这样会有一些问题，有些改变是只想影响一个实例。这里改变一个Symbol下的Sprite会影响到所有用到这个Symbol地方的节点。比如把第1棵树的第2根树枝的第3片叶子Sprite改成黄色，所有树枝的第3片叶子都变成了黄色的。为了解决这个问题，引入了Actor这层，Actor是Sprite实例化出来的，每个Sprite有0到n个Actor，每个Actor只有1个Sprite。Actor不会被复用，对应到一条具体的路径（Sprite-Symbol-Sprite-Symbol-Sprite-Symbol...）。Actor记录某个Sprite被改变的信息，并且这个改变只想影响到Sprite的某一个实例。就可以只把第1棵树的第2根树枝的第3片叶子改成黄色，其他树枝的第3片叶子不会受到影响。


## 这种三层结构合适or不适合用于那种情况？

游戏中需要改动的节点数量接近实际实例化出来的总数量时就不适用了，比如之前那个例子里要把1000片叶子设置成1000种不同的颜色，这样创建的Actor数量就和两层结构中创建的DNode的数量一样多了，非但没有起到优化的作用，还会更浪费（Sprite中的一些信息），而且变得更复杂了。


## 三层就够用了么？会不会还有重复的地方可以提出来，变成4层、5层？

并不是通过多层加一层来解决[两层结构中遇到的问题](#twolayer)，而是通过SNode指向DNode来去掉重复，Actor这层只是为了解决这样去掉重复时遇到的一些问题。Actor这级已经对应到最后实例化出来的对象不同的地方，这里没有重复。对于场景图来说这样的三层结构已经够用了。


## Sprite这级存了节点位置，Actor这级也有，这有些重复了吧，要加一个东西的话Sprite要加，Actor也要加。为什么不统一放在Actor这一级，每个Sprite创建一个默认的Actor，这样存取这类信息的话只用关心Actor？

这类接口的意义是不一样的，不应该耦合在一起，这样分开结构更清晰。比如Sprite这级的SRT接口是改变同一类Symbol中子节点位置的，会影响到所有用到这个Symbol的地方，这种操作一般用在编辑器里。Actor这级的SRT接口只是改具体某一个Sprite的位置，不想所有用到它的Symbol都受影响，这种操作一般用在游戏运行时。

同样的一类数据会有封装，比如动画信息存在AnimCurr里面，增加内容只需要改动这里。AnimSprite和AnimActor会有各自的AnimCurr，只是增加了一些接口。


## 跟ejoy2d相比的的优势？

ejoy2d里不能动态改变场景图的结构，只能从spritepack中读取打包好的结构，用mount来替换某个节点。sprite2中可以通过改变Symbol中Sprite子节点的组成来实现。

ejoy2d的场景图是两层结构spritepack和sprite对应SNode和DNode，[sprite会有大量重复](#twolayer)。[在sprite2里SNode会指向DNode，从而消除了这种重复](#spr_sym)。

节点有AABB信息，可以在渲染和查询时来减枝。

维护更方便，封装的更好，增加一个节点类型，就只需要实现它自己的一些接口，不需要影响到别的地方的代码。

扩展更方便，每一种节点都可以作为组件被其他节点使用。

资源管理更简洁高效。资源统一通过Sprite的引用计数来管理。资源用到了才被创建出来，比如ImageSymbol中的贴图，MeshSymbol中的mesh数据等，随着各自的Sprite的创建才会被创建出来。资源不用时可以立即被释放，Symbol不被任何Sprite引用时会清除自己占有的资源。