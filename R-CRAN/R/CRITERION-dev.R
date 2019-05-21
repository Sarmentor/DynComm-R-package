########################### Developer Notice ###########################
# Description:
#
# This file holds the developer documentation of new DynComm Criterion for main 
# algorithms.
#
# More developer information can be found in the project source page on GitHub at
# https://github.com/softskillsgroup/DynComm-R-package
#
#
# Author: poltergeist0
# Date: 2019-01-01

########################### Package Developer Documentation ###########################
#' @name CRITERION-dev
#' 
#' @title DynComm Documentation for Developers of new Criterion for Main Algorithms
#' 
#' @author poltergeist0
#' 
#' @rdname CRITERION-dev
#' 
#' @description 
#' Instructions for adding new criterion for main algorithms to the DynComm package.
#' 
#' @details 
#' Due to criterion being tightly connected to main algorithms and the multiple
#' programming language nature of this package, it is not possible to completely 
#' separate criterion from main algorithms to have them be independent.
#' 
#' This is due to the fact that criterion require access to the graph and, being
#' completely independent, would mean that another copy of the graph would have to
#' be stored at all times in the criterion to account for the possibilty of, as 
#' an example, using a main algorithm written in Python with a criterion written
#' in C++.
#' 
#' Even if there could be a way for all algorithms and criterion to share the 
#' same graph through R, performance would suffer because of all calls being
#' redirected through R.
#' 
#' So, currently, the best way is to write the criterion in each language. That
#' has the obvious downside of having to know several languages but, on the other 
#' side, criterion are fairly simple when compared to main algorithms.
#' 
#' Also, if you are not able to implement the criterion in every language, you 
#' can document which main algorithms support it by adding the name of the 
#' criterion to the documentation of the main algorithm under the "Supported 
#' CRITERION" section.
#' 
#' Depending on the programming language, follow the instructions below:
#' \describe{
#   \item{R}{
#   }
#'   \item{C++11}{
#' It might be easier to just copy the source code of an existing criterion an
#' modify it as required.\cr
#' Implement a class that extends class "CriterionBase" and implements all 
#' functions of class "CriterionInterface".
#' Follow the directions in the file "criterion.h" under the markers that start
#' with the word "TODO", to add your criterion to C++.
#'   }
#   \item{Python}{
#   }
#' }
#' 
#' Document your criterion in the file "CRITERION.R" following the instructions
#' stated in the header. Follow the example of other documented criterion and
#' add a section that states the supported main algorithms.
#' 
#' Add your criterion to the list of criterion in the file "DynCommMain.R" under
#' the marker that says "list new criterions here" and add a link to its
#' documentation in the documentation above that marker.
#' 
#' Add links to the criterion documentation on each of the supported main 
#' algorithms in the file "ALGORITHM.R"
#' 
#' @seealso \code{\link{DynComm}} , \code{\link{DynComm-package-dev}}
#' 
#' 
NULL

