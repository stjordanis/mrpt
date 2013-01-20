/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2013, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2013, MAPIR group, University of Malaga                |
   | Copyright (c) 2012-2013, University of Almeria                            |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */
#ifndef ScanMatching_H
#define ScanMatching_H

#include <mrpt/math.h>   // These 2 headers, in this order, are needed to avoid
#include <mrpt/poses.h>  //  undefined classes errors in inline constructors of mrpt::poses classes.

#include <mrpt/utils/TMatchingPair.h>

#include <mrpt/scanmatching/link_pragmas.h>

namespace mrpt
{
	namespace poses
	{
		class	CPosePDFParticles;
		class	CPosePDFGaussian;
		class	CPosePDFSOG;
	}

	/** A set of scan matching-related static functions.
	 * \sa mrpt::slam::CICP
	 * \ingroup mrpt_scanmatching_grp
	 */
	namespace scanmatching
	{
		using namespace mrpt::poses;
		using namespace mrpt::math;
		using namespace mrpt::utils;

		/** \addtogroup mrpt_scanmatching_grp
		  * @{ */

		/** This function implements the Horn method for computing the change in pose between two coordinate systems
		  * \param[in] inPoints		A vector containing the coordinates of the input points in the format:
		  *							[x11 y11 z11, x12 y12 z12, x21 y21 z21, x22 y22 z22, x31 y31 z31, x32 y32 z32, ...  ]
		  *							where [xi1 yi1 zi1] and [xi2 yi2 zi2] represent the i-th pair of corresponding 3D points in the two coordinate systems "1" and "2"
		  * \param[out] outQuat	A 7D vector containing the traslation and rotation (in a quaternion form) which indicates the change in pose of system "2" wrt "1".
		  * \param[in]  forceScaleToUnity	Whether or not force the scale employed to rotate the coordinate systems to one (rigid transformation)
		  *
		  * \return The computed scale of the optimal transformation (will be 1.0 for a perfectly rigid translation + rotation).
		  * \sa THornMethodOpts
		  */
		double SCANMATCHING_IMPEXP HornMethod(
			const vector_double  &inPoints,
			vector_double        &outQuat,
			bool                 forceScaleToUnity = false );

		//! \overload
		double SCANMATCHING_IMPEXP HornMethod(
			const vector_double      &inPoints,
			mrpt::poses::CPose3DQuat &outQuat,
			bool                      forceScaleToUnity  = false);

		/** This method provides the closed-form solution of absolute orientation using unit quaternions to a set of over-constrained correspondences for finding the 6D rigid transformation between two cloud of 3D points.
		  *  The output 3D pose is computed using the method described in "Closed-form solution of absolute orientation using unit quaternions", BKP Horn, Journal of the Optical Society of America, 1987.
		  *
		  * \param in_correspondences The set of correspondences in TMatchingPairList form ("this" and "other").
		  * \param out_transformation The change in pose (CPose3DQuat) of the "other" reference system wrt "this" reference system which minimizes the mean-square-error between all the correspondences.
		  * \exception Raises a std::exception if the list "in_correspondences" has not a minimum of three correspondences.
		  * \return True if there are at least three correspondences, or false otherwise, thus we cannot establish any correspondence.
		  *  Implemented by FAMD, 2007. Revised in 2010.
		  * \sa robustRigidTransformation
		  */
		bool SCANMATCHING_IMPEXP leastSquareErrorRigidTransformation6D(
			const TMatchingPairList	&in_correspondences,
			CPose3DQuat							&out_transformation,
			double								&out_scale,
			const bool 							forceScaleToUnity = false );

		/** This method provides the closed-form solution of absolute orientation using unit quaternions to a set of over-constrained correspondences for finding the 6D rigid transformation between two cloud of 3D points.
		  *  The output 3D pose is computed using the method described in "Closed-form solution of absolute orientation using unit quaternions", BKP Horn, Journal of the Optical Society of America, 1987.
		  *
		  * \param in_correspondences The set of correspondences.
		  * \param out_transformation The change in pose (CPose3DQuat) of the "other" reference system wrt "this" reference system which minimizes the mean-square-error between all the correspondences.
		  * \exception Raises a std::exception if the list "in_correspondences" has not a minimum of two correspondences.
		  * \return True if there are at least two correspondences, or false if one or none, thus we cannot establish any correspondence.
		  *  Implemented by FAMD, 2007. Revised in 2010
		  * \sa robustRigidTransformation
		  */
		bool SCANMATCHING_IMPEXP leastSquareErrorRigidTransformation6D(
			const TMatchingPairList	&in_correspondences,
			CPose3D								&out_transformation,
			double								&out_scale,
			const bool 							forceScaleToUnity = false );

		/** This method provides the closed-form solution of absolute orientation using unit quaternions to a set of over-constrained correspondences for finding the 6D rigid transformation between two cloud of 3D points using RANSAC.
		  *  The output 3D pose is computed using the method described in "Closed-form solution of absolute orientation using unit quaternions", BKP Horn, Journal of the Optical Society of America, 1987.
		  *  If supplied, the output covariance matrix is computed using... TODO
		  * \todo Explain covariance!!
		  *
		  * \param in_correspondences The set of correspondences.
		  * \param out_transformation The pose that minimizes the mean-square-error between all the correspondences.
		  * \param out_scale The estimated scale of the rigid transformation (should be very close to 1.0)
		  * \param out_inliers_idx Indexes within the "in_correspondences" list which corresponds with inliers
		  * \param ransac_minSetSize The minimum amount of points in the set
		  * \param ransac_nmaxSimulations The maximum number of iterations of the RANSAC algorithm
		  * \param ransac_maxSetSizePct The (minimum) assumed percent (0.0 - 1.0) of the input set to be considered as inliers
		  * \exception Raises a std::exception if the list "in_correspondences" has not a minimum of two correspondences.
		  * \return True if there are at least two correspondences, or false if one or none, thus we cannot establish any correspondence.
		  *  Implemented by FAMD, 2008.
		  * \sa robustRigidTransformation
		  */
		bool SCANMATCHING_IMPEXP leastSquareErrorRigidTransformation6DRANSAC(
			const TMatchingPairList	&in_correspondences,
			CPose3D								&out_transformation,
			double								&out_scale,
			vector_int							&out_inliers_idx,
			const unsigned int					ransac_minSetSize = 5,
			const unsigned int					ransac_nmaxSimulations = 50,
			const double						ransac_maxSetSizePct = 0.7,
			const bool							forceScaleToUnity = false );


		/** This method provides the basic least-square-error solution to a set of over-constrained correspondences for finding the (x,y,phi) rigid transformation between two planes.
		  *  The optimal transformation q fulfills:   \f$ point_this = q \oplus point_other \f$
		  * \param in_correspondences The set of correspondences.
		  * \param out_transformation The pose that minimizes the mean-square-error between all the correspondences.
		  * \param out_estimateCovariance If provided (!=NULL) this will contain on return a 3x3 covariance matrix with the NORMALIZED optimal estimate uncertainty. This matrix must be multiplied by \f$\sigma^2_p\f$, the variance of matched points in \f$x\f$ and \f$y\f$ (see paper http://www.mrpt.org/Paper:Occupancy_Grid_Matching)
		  * \exception Raises a std::exception if the list "in_correspondences" has not a minimum of two correspondences.
		  * \return True if there are at least two correspondences, or false if one or none, thus we cannot establish any correspondence.
		  * \sa robustRigidTransformation
		  */
		bool SCANMATCHING_IMPEXP leastSquareErrorRigidTransformation(
			TMatchingPairList	&in_correspondences,
			CPose2D							&out_transformation,
			CMatrixDouble33					*out_estimateCovariance = NULL );

		/** This method provides the basic least-square-error solution to a set of over-constrained correspondences for finding the (x,y,phi) rigid transformation between two planes.
		  *  The optimal transformation q fulfills:   \f$ point_this = q \oplus point_other \f$
		  * \param in_correspondences The set of correspondences.
		  * \param out_transformation The pose that minimizes the mean-square-error between all the correspondences.
		  * \param out_estimateCovariance If provided (!=NULL) this will contain on return a 3x3 covariance matrix with the NORMALIZED optimal estimate uncertainty. This matrix must be multiplied by \f$\sigma^2_p\f$, the variance of matched points in \f$x\f$ and \f$y\f$ (see paper http://www.mrpt.org/Paper:Occupancy_Grid_Matching)
		  * \exception Raises a std::exception if the list "in_correspondences" has not a minimum of two correspondences.
		  * \return True if there are at least two correspondences, or false if one or none, thus we cannot establish any correspondence.
		  * \sa robustRigidTransformation
		  */
		bool SCANMATCHING_IMPEXP leastSquareErrorRigidTransformation(
			TMatchingPairList	&in_correspondences,
			CPosePDFGaussian				&out_transformation );

		/** This method implements a RANSAC-based robust estimation of the rigid transformation between two planar frames of references, returning a probability distribution over all the posibilities as a Sum of Gaussians.
		  * 
		  *  The technique was described in the paper:
		  *		- J.L. Blanco, J. Gonz�lez-Jimenez and J.A. Fernandez-Madrigal. "A robust, multi-hypothesis approach to matching occupancy grid maps". Robotica, available on CJO2013. doi:10.1017/S0263574712000732. http://journals.cambridge.org/action/displayAbstract?aid=8815308
		  *
		  * This works are follows:
				- Repeat "ransac_nSimulations" times:
					- Randomly pick TWO correspondences from the set "in_correspondences".
					- Compute the associated rigid transformation.
					- For "ransac_maxSetSize" randomly selected correspondences, test for "consensus" with the current group:
						- If if is compatible (ransac_mahalanobisDistanceThreshold), grow the "consensus set"
						- If not, do not add it.
		  *
		  *  For more details refer to the tutorial on <a href="http://www.mrpt.org/Scan_Matching_Algorithms">scan matching methods</a>.
		  *  NOTE:
		  *        - If a pointer is supplied to "out_largestSubSet", the largest consensus sub-set
		  *          of correspondences will be returned there.
		  *        - The parameter "normalizationStd" is the <b>standard deviation</b> (not variance) of landmarks
		  *          being matched in X,Y. Used to normalize covariances returned as the SoG.
		  *        - If ransac_nSimulations=0 then an adaptive algorithm is used to determine the number of iterations, such as
		  *           a good model is found with a probability p=0.999, or that passed as the parameter probability_find_good_model
		  *        - When using "probability_find_good_model", the minimum number of iterations can be set with "ransac_min_nSimulations".
		  *
		  *  If ransac_fuseByCorrsMatch=true (the default), the weight of Gaussian modes will be increased when an exact match in the
		  *   subset of correspondences for the modes is found. Otherwise, an approximate method is used as test by just looking at the
		  *   resulting X,Y,PHI means (Threshold in this case are: ransac_fuseMaxDiffXY, ransac_fuseMaxDiffPhi).
		  *
		  * \exception Raises a std::exception if the list "in_correspondences" has not a minimum of two correspondences.
		  * \sa leastSquareErrorRigidTransformation
		  */
		void SCANMATCHING_IMPEXP robustRigidTransformation(
			TMatchingPairList	&in_correspondences,
			poses::CPosePDFSOG				&out_transformation,
			float							normalizationStd,
			unsigned int					ransac_minSetSize = 3,
			unsigned int					ransac_maxSetSize = 20,
			float							ransac_mahalanobisDistanceThreshold = 3.0f,
			unsigned int					ransac_nSimulations = 0,
			TMatchingPairList		*out_largestSubSet = NULL,
			bool						ransac_fuseByCorrsMatch = true,
			float						ransac_fuseMaxDiffXY = 0.01f,
			float						ransac_fuseMaxDiffPhi = DEG2RAD(0.1f),
			bool						ransac_algorithmForLandmarks = true,
			double 						probability_find_good_model = 0.999,
			unsigned int				ransac_min_nSimulations = 1500,
			const bool                  verbose = false
			);


		/** @} */  // end of grouping

	}

} // End of namespace

#endif
