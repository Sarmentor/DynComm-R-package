########################### Developer Notice ###########################
# Description:
#
# This file holds the user friendly documentation of all available DynComm post 
# processing algorithms.
#
# Development documentation of new post processing algorithms should be added 
# beside the algorithm implementation.
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

#' @name POSTPROCESSING_DENSOPT
#' 
#' @title POSTPROCESSING_DENSOPT
#'
#' @author poltergeist0
#' 
#' @description 
#' Implementation of the density optimization algorithm as a post processing
#' algorithm.
#' 
# @details 
#' 
#'
#' @rdname POSTPROCESSING_DENSOPT
#' 
#' @seealso \code{\link{DynComm}}
#' 
#' @section Performance:
#' \describe{
#'   \item{Initialization}{
#'   Uses a matrix with three columns and a maximum of verticelAll()^2 rows 
#'   with the edges between vertices and their weight (vertex<->vertex<->weight)
#'   of the original graph.
#'   Temporarily stores a copy of the graph to calculate a new community mapping.
#'   }
#'   \item{Results}{
#'   Uses a matrix with two columns and verticesAll() rows with the new community
#'   mapping (vertex<->community).
#'   Uses a matrix with three columns and a maximum of 
#'   communityCount()^2+communityCount() rows with the edges between communities 
#'   and their weight (community<->community<->weight).
#'   }
#' }
#' 
#' #' @section PARAMETERS:
#' This post processing algorithm does not require any parameters.
#' 
#' 
NULL