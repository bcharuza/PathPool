# PathPool

## Rationale

PathPool concept aims to minimize computational costs for storing and operating on multiple path entries in the system.

Each path in any given namespace must have following qualities:

 1. have parent path
 2. share a common prefix ( also empty prefix )
 3. provide means to explicitly compare any two paths

Each path can be described as:

 1. string describing full path
 2. linked list of tags describing full path
 3. a pair of:

	* reference to parent path
	* tag - element expanding parent path

## Implementation

The most memory efficient solution is a pool of paths described as pairs <parent path, expanding tag>. It allows for sharing prefixes for all paths in a pool so adding a path has memory complexity of O(1), as new memory is allocated only for the expanding tag - instead the whole path. Pair <parent path, expanding tag> can uniquely describe a path and in consequence be coupled to unique path id.
Path pools describe its paths similar to flyweight design pattern. Despite its memory savings flyweights, have also optimal computational cost for copying, moving, and equality-comparison operations. Efficiency loss occurs on path-string retrieval.

A design emerges where each node is defined once, and sub-paths contain only reference to their parent node - this leads to a centralized path pool register with tree-like structures.
Each path is a list with head pointing at its least significant tag. Head tag may be shared only with its subpaths. Head may be accessed using its id (`pathid_t`). Next node in the list is head's parent node. Last element of a list is root path shared by all paths in the pool. To allow for path operations, and local traversal, helper `get_*` methods are used.

## PathPools

All PathPool classes share a common interface - it is not formalized, as no inheritance is used.

### Example code

#### Basic usage

 * Ex1. Below is a basic example of how paths can be represented with single IDs (flyweight objects). For convenience it is better to use Path overlay object described below.

	`using pathid_t = ListPathPool<std::string>::pathid_t;`  
	`ListPathPool<std::string> pool { "root" };`  
	`pathid_t root = pool.get_root();`  
	`pathid_t p1 = pool.get_subnode(root, "path1");`  
	`pathid_t p2 = pool.get_subnode(root, "path2");`  
	`pathid_t p3 = pool.get_subnode(root, "path1");`  
	`pathid_t p4 = pool.get_subnode(p2, "path1");`  
	`// Notice no pool object is needed`  
	`std::cout<< (p1 == p2) << std::endl; // false`  
	`std::cout<< (p1 == p3) << std::endl; // true`  
	`std::cout<< (p1 == p4) << std::endl; // false`  
	`std::cout<< pool.get_tag(root) << std::endl; // root`  
	`std::cout<< pool.get_tag(p4) << std::endl; // path1`  
	`std::cout<< pool.get_tag(pool.get_parent(p4)) << std::endl; // path2`  
	
### Defined types:

  * `tag_t = TagT`
  
  Defines types paths are composed of. Type must allow for:

	* copying
	* equality comparison
	* (for HashPathPool) hashing
  
  * `pathid_t`
  
  Defines path-id which explicitly describes a path. Allows for:

	* path identification in the pool.
	* comparison
	* hashing
	* value operations like assignment, copying

### Methods:

  * `constructor()`
  
	Create pool with default root path - only if `tag_t` provides default constructor
	
  * `constructor(tag_t root)`
  
  * `get_subnode(pathid_t path,tag_t subnode) -> pathid_t`
  
	Returns a path described by `pair<path,subnode>`. Path is created if it doesn't exist
	
  * `get_subnodes(pathid_t path) const -> std::pair<iterator_t,iterator_t>`
  
	Returns a range of iterators to subpath list
	
  * `get_subnodes<ContainerT>(pathid_t path) const -> ResultT`
  
  	Returns all existing sub-paths in any given container supporting `std::back_inserter`
	
  * `get_parent(pathid_t path) const -> pathid_t`
  
	Returns a parent of a given path
	
  * `get_tag(pathid_t path) const -> tag_t`

	Returns a tag associated with a given path
	
  * `get_root() const noexcept -> pathid_t`

	Returns root path

### Global functions:

  * `get_taglist(PathPool,pathid_t) -> std::vector<tag_t>`

    Returns list of tags_t describing full path name in reverse order (from the least to the most significant tag)

  * `get_common_path(pathid_t,pathid_t, PathPool) -> std::array<pathid_t,3>`

    Returns 3 elements describing connection point of 2 paths `{common_node, left_subnode, right_subnode}`
	
### Implementations

#### ``HashPathPool<TagT, AllocatorT, HashF, EqualsF>``
 
 PathPool class using hash-map to store subnodes. Faster for pools with large number of subnodes with the same parent
	 
 * `TagT` 
 
 Stored type. `tag_t = TagT`
 
 * `AllocatorT = std::allocator`
 
 STL Allocator
 
 * `HashF = std::hash<TagT>`
 
 Hashing function.
 
 * `EqualsF = std::equal_to<TagT>`

 Equality predicate

#### ``ListPathPool<TagT, AllocatorT>``
 
 PathPool class uses array linked-lists to store subnodes. Faster in most cases.
	 
 * `TagT` 
 
 Stored type. `tag_t = TagT`

 * `AllocatorT = std::allocator`
 
 STL Allocator

## Path class

 Path class is used as object representation of PathPool::pathid_t, which is usually a basic type.
 Path limits operations to only secure ones. It hides pool object and prevents from mixing ids from different path pools.

### Example code

#### Basic usage

 * Ex1. Using Path objects leads to more readable and more secure code. Below is Path example equivalent to PathPool-Ex1

	`using Path = Path<ListPathPool<std::string>>;`  
	`Path root;`  
	`Path p1 { root, "path1" };`  
	`Path p2 { root, "path2" };`  
	`Path p3 { root, "path1" };`  
	`Path p4 { p2, "path1" };`  
	`std::cout<< (p1 == p2) << std::endl; // false`  
	`std::cout<< (p1 == p3) << std::endl; // true`  
	`std::cout<< (p1 == p4) << std::endl; // false`  
	`std::cout<< root.get_tag() << std::endl; // ''`  
	`std::cout<< p4.get_tag() << std::endl; // path1`  
	`std::cout<< p4.get_parent().get_tag() << std::endl; // path2`  

### ``Path<PoolT,int poolno = 0>``

Object representation of `pathid_t`. Takes only a space of `sizeof(pathid_t)`, and shares `PathPool` object for all its types. User cannot access `PathPool` object. Tag of root path. holds default value.

 * `PoolT` - concrete PathPool class
 * `poolno` - pool number. Allows creation of multiple pools of the same PoolT.
 
Example: `Path<ListPathPool<int>,0> root_1;`  
Example: `Path<ListPathPool<int>,1> root_2;`  
Example: `Path<HashPathPool<int>,0> root_3;`  

`root_1`,`root_2`,`root_3` represent 3 different types and no interaction between them is possible.

### Defined types

 * `tag_t = PathPool::tag_t`
 
 Defines type used for creating subpaths. tag_t must have default constructor.
 
 * `vertical_iterator`

 Forward-iterator used for traversing the path up.

 * `horizontal_iterator`

 Forward-iterator used for traversing the paths of the same parent.

### Methods

 * `constructor()`
 
 Creates root path.
 
 * `constructor(const Path&,const tag_t&)`
 
 Creates subpath.
 
 * `operator / (const tag_t&) const -> Path`
 
 Creates subpath.
 
 * `get_subpaths() -> std::pair<horizontal_iterator,horizontal_iterator>`
 
 Returns all subpaths referenced from the called path.
 
 * `get_parent() const -> Path`
 
 Returns parent of the called object
 
 * `get_tag() -> tag_t`
 
 Returns tag given on path creation.
 
 * `begin() -> vertical_iterator`

 Returns iterator pointing at called Path.

 * `cbegin() -> vertical_iterator`
 
 Returns iterator pointing at called Path.
 
 * `end() -> vertical_iterator`

 Returns iterator pointing at root Path.

 * `cend() -> vertical_iterator`
 
 Returns iterator pointing at root Path.
 
### Global functions

 * `operator == (const Path&, const Path&) -> bool`
 
 * `operator < (const Path&, const Path&) -> bool`

 * `common_path(const Path&, const Path&) -> Path`

 Returns common path of its two arguments.
