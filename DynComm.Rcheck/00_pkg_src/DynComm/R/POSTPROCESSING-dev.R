########################### Developer Notice ###########################
# Description:
#
# This file holds the developer documentation of new DynComm Post Processing
# Algorithms.
#
# More developer information can be found in the project source page on GitHub at
# https://github.com/softskillsgroup/DynComm-R-package
#
#
# Author: poltergeist0
# Date: 2019-01-01

########################### Package Developer Documentation ###########################
#' @name POSTPROCESSING-dev
#' 
#' @title DynComm Documentation for Developers of new Post Processing Algorithms
#' 
#' @author poltergeist0
#' 
#' @rdname POSTPROCESSING-dev
#' 
#' @description 
#' Instructions for adding new Post Processing Algorithms to the DynComm package.
#' 
# @details 
#' 
#' 
#' @section Steps:
#' This section provides step by step instructions on how to implement a new post 
#' processing algorithm and how to integrate it into the DynComm package.
#' \enumerate{
#'   \item
#' Go to the project source and get an appropriate template for your algorithm
#' from the "dev" folder on the root of the project source.\cr
#' Different languages are distinguished by the extension of the file.\cr
#' If a template is not available use one from a language that is most related 
#' to the language you intend to use or use the R template to, at least, 
#' provide you with the function names, types of inputs and types of outputs.
#'   \item
#' Implement the new algorithm inside the template, preferably, inside a 
#' private function. The algorithm, that is the private function, must be 
#' called at the end of the constructor.\cr
#' All API functions must only be used to convert between data types and to 
#' return data. No calculations should be performed in them.
#'     \describe{
#'       \item{R}{
#' Temporarily add any library required by the algorithm in your source file. It 
#' will have to be removed later but, for now, it is useful for testing.
#'       }
#'     }
#' Depending on the programming language used, do the following:
#'     \describe{
#'       \item{R}{
#' Choose the "TemplateDynCommPostProcess.R" template.\cr
#' Save the source file of the new algorithm in the "R-CRAN/R" folder.
#'       }
#       \item{C++11}{
# Choose the "TemplateDynCommPostProcess.h" template.\cr
# Save the source file of the new algorithm in the "R-CRAN/src/base/Cpp" folder. 
#       }
#       \item{Python}{
# Choose the "TemplateDynCommPostProcess.py" template.\cr
# Save the source file of the new algorithm in the "R-CRAN/src/base/Python" folder. 
#       }
#'     }
#' The name of the file should reflect the name of the algorithm and start with 
#' the word "postProcess".
#'   \item
#' Test the algorithm independently of the DynComm package. If programming in...
#'     \describe{
#'       \item{R}{
#' You can "source" your R file.
#'       }
#       \item{C++11}{
# The easiest option is to integrate your post processing algorithm in the 
# existing C++ post processing algorithm class named "PostProcessBase".
#       }
#       \item{Python}{
# You can test it by invoking it directly from a python command line. Check Python
# documentation on how to do it.
#       }
#'     }
#' Use an applicable main algorithm of the DynComm package to generate data to
#' your post processing algorithm.
#' Use the tests performed as examples and write them in the documentation when it
#' is created.
#'   \item
#' If the algorithm you are creating has associated bibliography, add reference to
#' it in the existing "REFERENCES.bib" file.
#'   \item
#' Create documentation for your algorithm. This involves adding documentation in 
#' three files.\cr
#' The first is the developer documentation on the same file you implemented your 
#' algorithm. If using a template, there is already a documentation template for
#' you to modify.\cr
#' This documentation must have a detailed description of the algorithm, including
#' a description of how it works, its parameters and contain examples that can be 
#' used for automatic testing.\cr
#' The second file is the "POSTPROCESSING.R" where the user friendly documentation 
#' is written. This documentation should have a high level description of the 
#' algorithm, acceptable parameters and resource utilization.\cr
#' The thrird file is the "DynCommPostProcess.R" where it says "document new 
#' algorithms here". This documentation should use the same format used for other 
#' algorithms with a very small description of the algorithm, preferably just two 
#' lines, with a link to the user friendly documentation and references to 
#' publications.
#'   \item
#' Add the name of your algorithm to the POSTPROCESSING list in the 
#' "DynCommPostProcess.R" file under the marker that says "list new algorithms 
#' here".\cr
#' Add your algorithm parameters to the matrix in the "DynComm.R" file under the 
#' marker that says "add parameters here".
#'     \describe{
#'       \item{R}{
#' Add a source command to the "DynCommPostProcess.R" file under the marker that 
#' says "Include R sources here".
#' Add any R libraries required by your algorithm to the "DynComm.R" file under
#' the marker that says "List imports here".\cr
#' Remove all libraries from your algorithms' R source file.
#'       }
#       \item{C++11}{
# Edit the file "postProcessBase.h" and look for the several "...new algorithms here"
# markers. They will tell what needs to be done to integrate your algorithm.\cr
# Then, if your algorithm requires any parameters, edit the file "program.h" and 
# look for the several "...new algorithms here" markers. They will tell what 
# needs to be done to add your algorithms' parameters to the C++ source.
#       }
#       \item{Python}{
# TO DO :(
#       }
#'     }
#'   \item{
#' You should now be able to build the package and, if everything went right, your
#' algorithm is successfuly integrated into this package. \strong{Congratulations :D}
#'   }
#' }
#' 
#' @seealso \code{\link{DynComm}} , \code{\link{DynComm-package-dev}}
#' 
#' 
NULL

