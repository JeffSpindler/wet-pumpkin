// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/11/02)
// Define a ray pair multi-index set container

#ifndef PAIRSORT2MISET_H
#define PAIRSORT2MISET_H

#include "Global.h"
#include "PairSort2.h"


// namespaces
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include "boost/multi_array.hpp"

using boost::multi_index_container;
using namespace boost::multi_index;

/* tags for accessing the corresponding indices of PairSort2 */
struct ps2_mi_tag{};
struct ps2_mi_idx{};
struct ps2_mi_src1{};
struct ps2_mi_src2{};
struct ps2_mi_src1_src2{};
struct ps2_mi_dist{};
struct ps2_mi_dist2{};
struct ps2_mi_dist_dist2{};

typedef multi_index_container<
  PairSort2,
  indexed_by<
    ordered_non_unique<
      tag<ps2_mi_tag>, BOOST_MULTI_INDEX_MEMBER(PairSort2,int,m_tag)>,
    ordered_non_unique<
      tag<ps2_mi_idx>, BOOST_MULTI_INDEX_MEMBER(PairSort2,int,m_idx)>,
    ordered_non_unique<
      tag<ps2_mi_src1>, BOOST_MULTI_INDEX_MEMBER(PairSort2,int,m_src1)>,
    ordered_non_unique<
      tag<ps2_mi_src2>, BOOST_MULTI_INDEX_MEMBER(PairSort2,int,m_src2)>,
    ordered_non_unique<
      tag<ps2_mi_src1_src2>, composite_key< PairSort2,
                         BOOST_MULTI_INDEX_MEMBER(PairSort2,int,m_src1),
                         BOOST_MULTI_INDEX_MEMBER(PairSort2,int,m_src2)> >,
    ordered_non_unique<
      tag<ps2_mi_dist>, BOOST_MULTI_INDEX_MEMBER(PairSort2,float,m_dist)>,
    ordered_non_unique<
      tag<ps2_mi_dist2>, BOOST_MULTI_INDEX_MEMBER(PairSort2,float,m_dist2)>,
    ordered_non_unique<
      tag<ps2_mi_dist_dist2>, composite_key< PairSort2,
                         BOOST_MULTI_INDEX_MEMBER(PairSort2,float,m_dist),
                         BOOST_MULTI_INDEX_MEMBER(PairSort2,float,m_dist2)> >
>
> ps2_mi_set;

typedef ps2_mi_set::index<ps2_mi_tag>::type ps2_mi_tag_index_t;
typedef ps2_mi_set::index<ps2_mi_idx>::type ps2_mi_idx_index_t;
typedef ps2_mi_set::index<ps2_mi_src1>::type ps2_mi_src1_index_t;
typedef ps2_mi_set::index<ps2_mi_src2>::type ps2_mi_src2_index_t;
typedef ps2_mi_set::index<ps2_mi_src1_src2>::type cand_mi_src1_src2_index_t;
typedef ps2_mi_set::index<ps2_mi_dist>::type ps2_mi_dist_index_t;
typedef ps2_mi_set::index<ps2_mi_dist2>::type ps2_mi_dist2_index_t;
typedef ps2_mi_set::index<ps2_mi_dist_dist2>::type cand_mi_dist_dist2_index_t;

template<typename Tag,typename MultiIndexContainer>
void print_ps2_by( const MultiIndexContainer& s, Tag* =0 )
{
  /* obtain a reference to the index tagged by Tag */

  const typename boost::multi_index::index<MultiIndexContainer,Tag>::type& i=
    get<Tag>(s);

  typedef typename MultiIndexContainer::value_type value_type;

  /* dump the elements of the index to cout */
  if(i.begin() == i.end()) {
	  return;
  }

  std::copy(i.begin(),i.end(),std::ostream_iterator<value_type>(std::cout));
}


#endif
