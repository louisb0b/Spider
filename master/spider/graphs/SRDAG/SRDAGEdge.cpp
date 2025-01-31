/****************************************************************************
 * Copyright or © or Copr. IETR/INSA (2013): Julien Heulot, Yaset Oliva,    *
 * Maxime Pelcat, Jean-François Nezan, Jean-Christophe Prevotet             *
 *                                                                          *
 * [jheulot,yoliva,mpelcat,jnezan,jprevote]@insa-rennes.fr                  *
 *                                                                          *
 * This software is a computer program whose purpose is to execute          *
 * parallel applications.                                                   *
 *                                                                          *
 * This software is governed by the CeCILL-C license under French law and   *
 * abiding by the rules of distribution of free software.  You can  use,    *
 * modify and/ or redistribute the software under the terms of the CeCILL-C *
 * license as circulated by CEA, CNRS and INRIA at the following URL        *
 * "http://www.cecill.info".                                                *
 *                                                                          *
 * As a counterpart to the access to the source code and  rights to copy,   *
 * modify and redistribute granted by the license, users are provided only  *
 * with a limited warranty  and the software's author,  the holder of the   *
 * economic rights,  and the successive licensors  have only  limited       *
 * liability.                                                               *
 *                                                                          *
 * In this respect, the user's attention is drawn to the risks associated   *
 * with loading,  using,  modifying and/or developing or reproducing the    *
 * software by the user in light of its specific status of free software,   *
 * that may mean  that it is complicated to manipulate,  and  that  also    *
 * therefore means  that it is reserved for developers  and  experienced    *
 * professionals having in-depth computer knowledge. Users are therefore    *
 * encouraged to load and test the software's suitability as regards their  *
 * requirements in conditions enabling the security of their systems and/or *
 * data to be ensured and,  more generally, to use and operate it in the    *
 * same conditions as regards security.                                     *
 *                                                                          *
 * The fact that you are presently reading this means that you have had     *
 * knowledge of the CeCILL-C license and that you accept its terms.         *
 ****************************************************************************/

#include <graphs/SRDAG/SRDAGCommon.h>
#include <graphs/SRDAG/SRDAGVertex.h>
#include <graphs/SRDAG/SRDAGEdge.h>

/** Static Var def */
int SRDAGEdge::globalId = 0;

SRDAGEdge::SRDAGEdge(){
	id_ = -1;
	graph_ = 0;

	src_ = 0; srcPortIx_ = -1;
	snk_ = 0; snkPortIx_ = -1;

	rate_ = -1;
	alloc_ = -1;
	allocIx_ = -1;
	nToken_ = 1;
}

SRDAGEdge::SRDAGEdge(SRDAGGraph* graph){
	id_ = globalId++;
	graph_ = graph;

	src_ = 0; srcPortIx_ = -1;
	snk_ = 0; snkPortIx_ = -1;

	rate_ = -1;
	alloc_ = -1;
	allocIx_ = -1;
	nToken_ = 1;
}

SRDAGEdge::~SRDAGEdge(){

}

void SRDAGEdge::connectSrc(SRDAGVertex *src, int srcPortId){
	if(src_ != 0)
		throw "SRDAGEdge: try to connect to an already connected edge";
	src_ = src;
	srcPortIx_ = srcPortId;
	src_->connectOutEdge(this, srcPortIx_);
}

void SRDAGEdge::connectSnk(SRDAGVertex *snk, int snkPortId){
	if(snk_ != 0)
		throw "SRDAGEdge: try to connect to an already connected edge";
	snk_ = snk;
	snkPortIx_ = snkPortId;
	snk_->connectInEdge(this, snkPortIx_);
}

void SRDAGEdge::disconnectSrc(){
	if(src_ == 0)
		throw "SRDAGEdge: try to disconnect a not connected edge";
	src_->disconnectOutEdge(srcPortIx_);
	src_ = 0;
	srcPortIx_ = -1;
}

void SRDAGEdge::disconnectSnk(){
	if(snk_ == 0)
		throw "SRDAGEdge: try to disconnect a not connected edge";
	snk_->disconnectInEdge(snkPortIx_);
	snk_ = 0;
	snkPortIx_ = -1;
}







