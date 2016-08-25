// you can write to stdout for debugging purposes, e.g.
// console.log('this is a debug message');

function solution(A, B, K) {
	// define a node object
	var Node = function(id) {
		this.id = id;
		this.height = 0;
		this.neighbours = [];
		this.root = null;
		this.children = [];
	}

	// create a network of nodes
	var network = [];
	for (var i = 0; i < A.length + 1; ++i) network.push(new Node(i));

	// read data and fill 'neighbours' field of nodes
	for (var i = 0; i < A.length; ++i) {
		network[A[i]].neighbours.push(network[B[i]]);
		network[B[i]].neighbours.push(network[A[i]]);
	}

	// spin out network into a tree by making an arbitrary node the base
	function spin_tree(node, root) {
		node.root = root;
		for (var child of node.neighbours) {
			if (child == root) continue;
			node.children.push(child);
			spin_tree(child, node);
		}
	}

	spin_tree(network[0], null);

	function needed_cameras(node, allowed_dist) {
		node.height = 0;
		var result = 0;
		for (var child of node.children) result += needed_cameras(child, allowed_dist);

		// copy out the child list and then sort by height of child node
		var child_list = node.children.slice();
		child_list.sort(function(a, b) { return a.height > b.height; });

		// checking
		//var string = "";
		//for (var child of child_list) string += child.id + ":" + child.height + " ";
		//console.log("I am " + node.id + ", my children are " + string);

		while (child_list.length > 1 && child_list[child_list.length - 1].height + child_list[child_list.length - 2].height + 2 > allowed_dist) {
			child_list.pop();
		}
		if (child_list.length > 0 && child_list[child_list.length - 1].height + 1 > allowed_dist) {
			child_list.pop();
		}

		// update the height of the node based on remaining children
		if (child_list.length > 0) node.height = child_list[child_list.length - 1].height + 1;

		result += node.children.length - child_list.length;
		return result;
	}

	var high = Math.min(A.length, 900);
	var low = 0;
	var res = [];
	while (low <= high) {
		var mid = parseInt((high + low) / 2);
		var needed = needed_cameras(network[0], mid);
		if (needed > K) low = mid + 1
		else {
			res.push(mid);
			high = mid - 1
		}
	}

	return Math.min.apply(null, res);
}
