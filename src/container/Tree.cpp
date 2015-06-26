#include "container/Tree.h"

#include <iostream>
using namespace std;

#ifdef ENABLE_TESTCASE
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
#include <debug/testing.h>
TESTCASE_START
{
    void* arg = NULL;
    TestCase& testcase = Sigleton<TestCase>();
    testcase.addTestCase ("Tree", [](void* arg){
        cout << "Go Testing" << __FILE__ << endl;

//        TreeNode<int> root(0);

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
    }, arg );
}

TESTCASE_END

#endif // ENABLE_TESTCASE
