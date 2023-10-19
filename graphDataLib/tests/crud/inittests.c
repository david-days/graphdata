//
// Created by david on 11/18/17.
//

#include <stdlib.h>
#include <check.h>
#include <graphinit.h>
#include <util/crudops.h>

/**
 * Test that the graphops initialization has been properly carried out.
 */
START_TEST(rawGraphopsTest) {
    struct graphops_t *gops = initGraphops();
    ck_assert_msg(NULL != gops,
                  "graph ops should not be null");
    ck_assert_msg(NULL == gops->g,
                  "gops->g should be null");
    ck_assert_msg(NULL == gops->addEdge,
                  "gops->addEdge should be null");
    ck_assert_msg(NULL == gops->addNode,
                  "gops->addNode should be null");
    ck_assert_msg(NULL == gops->edgeCount,
                  "gops->edgeCount should be null");
    ck_assert_msg(NULL == gops->getEdge,
                  "gops->getEdge should be null");
    ck_assert_msg(NULL == gops->getEdges,
                  "gops->getEdges should be null");
    ck_assert_msg(NULL == gops->getNeighbors,
                  "gops->getNeighbors should be null");
    ck_assert_msg(NULL == gops->getNode,
                  "gops->getNode should be null");
    ck_assert_msg(NULL == gops->nodeCount,
                  "gops->nodeCount should be null");
    ck_assert_msg(NULL == gops->resetGraph,
                  "gops->resetGraph should be null");
    ck_assert_msg(NULL == gops->setCapacity,
                  "gops->setCapacity should be null");
    free(gops);
}
END_TEST

/**
 * @brief Test the createDimensions() function returns expected structure
 *
 *
 */
START_TEST(dimTest)
        {
            size_t t1size = 3;
            size_t t1arr[3] = {1,2,3};
            size_t t2size = 5;
            size_t t2arr[5] = {10,20,30,40,50};
            size_t t3size = 2;
            size_t t3arr[2] = {255,255};

            //Start testing
            struct dimensions_t *dim1 = createDimensions(3,1,2,3);
            ck_assert_msg(NULL != dim1,
                          "Dim1 should not be null");
            ck_assert_msg(dim1->dimcount == t1size,
                            "Dim1 count should be equal to t1size");
            for (int i=0;i<t1size;i++) {
                ck_assert_msg(dim1->dimarr[i] == t1arr[i],
                              "dimension array did not match expected size");
            }
            free(dim1);

            struct dimensions_t *dim2 = createDimensions(5, 10,20,30,40,50);
            ck_assert_msg(NULL != dim2,
                          "Dim2 should not be null");
            ck_assert_msg(dim2->dimcount == t2size,
                          "Dim2 count should be equal to t2size");
            for (int i=0;i<t2size;i++) {
                ck_assert_msg(dim2->dimarr[i] == t2arr[i],
                              "dimension array did not match expected size");
            }
            free(dim2);

            struct dimensions_t *dim3 = createDimensions(2, 255,255);
            ck_assert_msg(NULL != dim3,
                          "dim3 should not be null");
            ck_assert_msg(dim3->dimcount == t3size,
                          "dim3 count should be equal to t3size");
            for (int i=0;i<t3size;i++) {
                ck_assert_msg(dim3->dimarr[i] == t3arr[i],
                              "dim3 array did not match expected size");
            }
            free(dim3);
        }
END_TEST


Suite * init_suite(void)
{
    Suite *s;
    TCase *tc_core;
    
    s = suite_create("Init");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, rawGraphopsTest);
    tcase_add_test(tc_core, dimTest);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = init_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}