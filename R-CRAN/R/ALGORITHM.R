########################### Developer Notice ###########################
# Description:
#
# This file holds the user friendly documentation of all available DynComm main 
# algorithms.
#
# Development documentation of new main algorithms should be added beside the 
# algorithm implementation.
#
# This documentation should not have examples, export, usage, format or return
# tags since those are not applicable by an end user.
#
# More developer information can be found in the project source page on GitHub at
# https://github.com/softskillsgroup/DynComm-R-package
#
#
# Author: poltergeist0
# Date: 2019-01-01

#' @name ALGORITHM_LOUVAIN
#' 
#' @title ALGORITHM_LOUVAIN
#'
#' @author poltergeist0
#' 
#' @description 
#' Is a greedy optimization method to extract communities from large networks 
#' by optimizing the density of edges inside communities to edges outside 
#' communities.
#' 
# @details 
#'
#' @rdname ALGORITHM_LOUVAIN
#' 
#' @seealso \code{\link{DynComm}}
#' 
#' @section Supported CRITERION:
#' \describe{
#'   \item{MODULARITY}{See \code{\link{CRITERION_MODULARITY}}}
#' }
#' 
#' @section PARAMETERS:
#' This algorithm does not require any parameters.
#' 
#' 
NULL
