#pragma once

template <typename MetricType, typename TreeType>
class EpsMinpts_Rules {

public:
	/**
	 * Construct the EpsMinpts_Rules object.  This is usually done from within
	 * the RangeSearch class at search time.
	 *
	 * @param referenceSet Set of reference data.
	 * @param querySet Set of query data, which is always the same with the referenceData
	 * @param dbscan_eps The search_range parameter of DBSCAN.
	 * @param dbscan_minpts The min_pts parameter of DBSCAN.
	 * @param neighbors Vector to store resulting neighbors in.
	 * @param distances Vector to store resulting distances in.
	 * @param metric Instantiated metric.
	 */
	EpsMinpts_Rules(const arma::mat& referenceSet,
					const arma::mat& querySet,
					const double dbscan_eps,
					const int dbscan_minpts,
					std::vector<std::vector<size_t> >& neighbors,
					std::vector<std::vector<double> >& distances,
					std::vector<bool>& isCore,
					MetricType& metric,
					const bool sameSet=false
					);

	/**
	 * Compute the base case between the given query point and reference point.
	 *
	 * @param queryIndex Index of query point.
	 * @param referenceIndex Index of reference point.
	 */
	
	double BaseCase(const size_t queryIndex, const size_t referenceIndex);
	
	/**
	 * Get the score for recursion order.  A low score indicates priority for
	 * recursion, while DBL_MAX indicates that the node should not be recursed
	 * into at all (it should be pruned).
	 *
	 * @param queryIndex Index of query point.
	 * @param referenceNode Candidate node to be recursed into.
	 */
	double Score(const size_t queryIndex, TreeType& referenceNode);

	/**
	 * Re-evaluate the score for recursion order.  A low score indicates priority
	 * for recursion, while DBL_MAX indicates that the node should not be recursed
	 * into at all (it should be pruned).  This is used when the score has already
	 * been calculated, but another recursion may have modified the bounds for
	 * pruning.  So the old score is checked against the new pruning bound.
	 *
	 * @param queryIndex Index of query point.
	 * @param referenceNode Candidate node to be recursed into.
	 * @param oldScore Old score produced by Score() (or Rescore()).
	 */
	double Rescore(const size_t queryIndex,
				   TreeType& referenceNode,
				   const double oldScore) const;

	/**
	 * Get the score for recursion order.  A low score indicates priority for
	 * recursion, while DBL_MAX indicates that the node should not be recursed
	 * into at all (it should be pruned).
	 *
	 * @param queryNode Candidate query node to recurse into.
	 * @param referenceNode Candidate reference node to recurse into.
	 */
	double Score(TreeType& queryNode, TreeType& referenceNode);

	/**
	 * Re-evaluate the score for recursion order.  A low score indicates priority
	 * for recursion, while DBL_MAX indicates that the node should not be recursed
	 * into at all (it should be pruned).  This is used when the score has already
	 * been calculated, but another recursion may have modified the bounds for
	 * pruning.  So the old score is checked against the new pruning bound.
	 *
	 * @param queryNode Candidate query node to recurse into.
	 * @param referenceNode Candidate reference node to recurse into.
	 * @param oldScore Old score produced by Score() (or Rescore()).
	 */
	double Rescore(TreeType& queryNode,
				   TreeType& referenceNode,
				   const double oldScore) const;

	typedef typename mlpack::TraversalInfo<TreeType> TraversalInfoType;

	const TraversalInfoType& TraversalInfo() const {
		return traversalInfo;
	}
	TraversalInfoType& TraversalInfo() {
		return traversalInfo;
	}


	//! Get the minimum number of base cases we need to perform to have acceptable
	//! results.
	size_t MinimumBaseCases() const {
		return 0;
	}

private:
	//! The reference set.
	const arma::mat& referenceSet;

	//! The query set.
	const arma::mat& querySet;

	//! The eps and minpts parameter of dbscan.
	const int dbscan_minpts;
	const double dbscan_eps;

	//! The vector the resultant neighbor indices should be stored in.
	std::vector<std::vector<size_t> >& neighbors;

	//! The vector the resultant neighbor distances should be stored in.
	std::vector<std::vector<double> >& distances;
	//! The vector storing whether each point is a core-point
	std::vector<bool>& isCore;
	//! The instantiated metric.
	MetricType& metric;

	//! If true, the query and reference set are taken to be the same.
	bool sameSet;

	//! The last query index.
	size_t lastQueryIndex;
	//! The last reference index.
	size_t lastReferenceIndex;

	//! Add all the points in the given node to the results for the given query
	//! point.  If the base case has already been calculated, we make sure to not
	//! add that to the results twice.
	void AddResult(const size_t queryIndex,
				   TreeType& referenceNode);

	TraversalInfoType traversalInfo;

	//! The number of base cases.
	//size_t baseCases;
	//! THe number of scores.
	//size_t scores;
};

#include "EpsMinpts_Rules_impl.h"