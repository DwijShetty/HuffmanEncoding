//
// Created by dwij on 4/14/2022.
//
#include <gtest/gtest.h>
#include "tests.h"
#include "mymap.h"
#include "hashmap.h"

TEST(hashmap, put){
    hashmap map;
    int arr[] = {2, 1, 3};

    for (int i = 0; i < 3; i++) {
        map.put(arr[i], arr[i]);
    }

    EXPECT_EQ(map.size(), 3);
}
TEST(hashmap, get) {
    hashmap map;
    int arr[] = {2, 1, 3};

    for (int i = 0; i < 3; i++) {
        map.put(arr[i], arr[i]);
    }

    for (int i = 0; i < 3; i++) {

        EXPECT_EQ(map.get(arr[i]), arr[i]);

        EXPECT_EQ(map.size(), 3);

    }
}

