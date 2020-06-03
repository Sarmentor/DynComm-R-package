devtools::use_testthat()

context("DynComm Test")

library(DynComm)


test_that("check results function", {
  expect_equal(results(), 1)
  
})


test_that("check addRemoveEdgesFile function", {
  expect_equal(addRemoveEdgesFile(), 1)
  
})


test_that("check addRemoveEdgesMatrix function", {
  expect_equal(addRemoveEdgesMatrix(), 1)
  
})


test_that("check quality function", {
  expect_equal(quality(), 1)
  
})


test_that("check communityCount function", {
  expect_equal(communityCount(), 1)
  
})


test_that("check communities function", {
  expect_equal(communities(), 1)
  
})


test_that("check communityNeighbours function", {
  expect_equal(communityNeighbours(), 1)
  
})


test_that("check communityInnerEdgesWeight function", {
  expect_equal(communityInnerEdgesWeight(), 1)
  
})


test_that("check communityTotalWeight function", {
  expect_equal(communityTotalWeight(), 1)
  
})


test_that("check communityEdgeWeight function", {
  expect_equal(communityEdgeWeight(), 1)
  
})


test_that("check communityVertexCount function", {
  expect_equal(communityVertexCount(), 1)
  
})


test_that("check community function", {
  expect_equal(community(), 1)
  
})


test_that("check vertexCount function", {
  expect_equal(vertexCount(), 1)
  
})


test_that("check verticesAll function", {
  expect_equal(verticesAll(), 1)
  
})


test_that("check vertices function", {
  expect_equal(vertices(), 1)
  
})


test_that("check communityMappingMatrix function", {
  expect_equal(communityMappingMatrix(), 1)
  
})


test_that("check communityMappingFile function", {
  expect_equal(communityMappingFile(), 1)
  
})

test_that("check neighbors function", {
  expect_equal(neighbors(), 1)
  
})


test_that("check edgeWeight function", {
  expect_equal(edgeWeight(source,destination), 1)
  
})


test_that("check edgeCount function", {
  expect_equal(edgeCount(), 1)
  
})

test_that("check time function",{
  expect_equal(time(), 1)

})