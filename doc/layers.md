SceneNode和component
表达节点组合关系的对象用SceneNode，scene node上面的其他特性用component来实现

SceneNode包括spr和sym，sym描述节点组合关系，spr是sym实例化后的对象，包括当前状态信息。
spr的component可以包含aabb，str，render color，render filter

scene nodes里面使用的组件
sym里面放模板的属性，比如complex的clipbx拿出来，作为sym的一种component





