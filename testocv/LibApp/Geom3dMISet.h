// Perform3-D LLC
// Copyright (c) 2013
// File Version: 1.0.0 (2013/09/15)
// Define a geom3d multi-index set container

#ifndef GEOM3DMISET_H
#define GEOM3DMISET_H

#include "Global.h"
#include "Geom3d.h"


// namespaces
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include "boost/multi_array.hpp"

using boost::multi_index_container;
using namespace boost::multi_index;

/* tags for accessing the corresponding indices of camft3d_set */
struct g3d_mi_tag{};
struct g3d_mi_type{};
struct g3d_mi_idx{};
struct g3d_mi_src{};
struct g3d_mi_src_idx{};

typedef multi_index_container<
  Geom3d,
  indexed_by<
    ordered_non_unique<
      tag<g3d_mi_tag>, BOOST_MULTI_INDEX_MEMBER(Geom3d,int,m_tag)>,
    ordered_non_unique<
      tag<g3d_mi_type>, BOOST_MULTI_INDEX_MEMBER(Geom3d,int,m_type)>,
    ordered_non_unique<
      tag<g3d_mi_idx>, BOOST_MULTI_INDEX_MEMBER(Geom3d,int,m_idx)>,
    ordered_non_unique<
      tag<g3d_mi_src>, BOOST_MULTI_INDEX_MEMBER(Geom3d,int,m_src)>,
    ordered_non_unique<
      tag<g3d_mi_src_idx>, composite_key< Geom3d,
                         BOOST_MULTI_INDEX_MEMBER(Geom3d,int,m_src),
                         BOOST_MULTI_INDEX_MEMBER(Geom3d,int,m_idx)> >
>
> g3d_mi_set;

typedef g3d_mi_set::index<g3d_mi_tag>::type g3d_mi_tag_index_t;
typedef g3d_mi_set::index<g3d_mi_type>::type g3d_mi_type_index_t;
typedef g3d_mi_set::index<g3d_mi_idx>::type g3d_mi_idx_index_t;
typedef g3d_mi_set::index<g3d_mi_src>::type g3d_mi_src_index_t;
typedef g3d_mi_set::index<g3d_mi_src_idx>::type cand_mi_src_idx_index_t;

template<typename Tag,typename MultiIndexContainer>
void print_g3d_by( const MultiIndexContainer& s, Tag* =0 )
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
