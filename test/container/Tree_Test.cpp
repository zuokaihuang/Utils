#ifdef ENABLE_TESTCASE
#include "container/Tree.h"
#include "debug/testing.h"
#include <iostream>
using namespace std;

/*
 *                root
 *                 1
 *              /  |  \
 *             2   4   5
 *            /      /   \
 *           3      6     7
 *                          \
 *                           8
 */

static int test_Tree(){
    cout << "Go Testing" << __FILE__ << endl;

    TreeNode<int> root( 1 );

    TreeNode<int> node2( 2 );
    TreeNode<int> node3( 3 );
    TreeNode<int> node4( 4 );
    TreeNode<int> node5( 5 );
    TreeNode<int> node6( 6 );
    TreeNode<int> node7( 7 );
    TreeNode<int> node8( 8 );

    TreeNode<int>::dump ( &root );
    TreeNode<int>::dump ( &node2 );
    TreeNode<int>::dump ( &node3 );
    TreeNode<int>::dump ( &node4 );
    TreeNode<int>::dump ( &node5 );
    TreeNode<int>::dump ( &node6 );
    TreeNode<int>::dump ( &node7 );
    TreeNode<int>::dump ( &node8 );

    root.insert ( &node2 );

    node2.insert ( &node3 );
    root.insert ( &node4 );

    root.insert ( &node5 );

    node5.insert ( &node6 );
    node5.insert ( &node7 );

    node7.insert ( &node8 );

    bool contain = TreeNode<int>::is_contains ( &node5, &node8 );

    cout << "constain?" << contain << endl;

    root.walk ( &root,[](int val){
        cout << "Hello:" << val << endl;
    } );

    return 0;
}

TC_Entry( Tree )

#endif // ENABLE_TESTCASE


