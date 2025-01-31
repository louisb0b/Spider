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

#include <tools/StaticStack.h>
#include <platform.h>

#include <stdio.h>
#include <algorithm>
#include <math.h>
//#include <unistd.h>

StaticStack::StaticStack(const char* name, void* ptr, int size):
		Stack(name) {
	size_ = size;
	stack_ = (char*)ptr;
	curPtr_ = (char*)ptr;
	maxUsed_ = 0;
	used_ = 0;
}

StaticStack::~StaticStack(){
	freeAll();
	printStat();
}

static inline int getAlignSize(int size){
	float minAlloc = Platform::get()->getMinAllocSize();
	return ceil(size/minAlloc)*minAlloc;
}

void *StaticStack::alloc(int size){
	size = getAlignSize(size);
	void* res;
	if(used_+size > size_)
		throw "Insufficient memory size of the Stack\n";
	res = curPtr_;
	curPtr_ += size;
	used_ += size;
	return res;
}

void StaticStack::free(void* var){
}

void StaticStack::freeAll(){
	maxUsed_ = std::max(maxUsed_, used_);
	curPtr_ = stack_;
	used_ = 0;
}


void StaticStack::printStat(){
	printf("%s: ", getName());

	if(maxUsed_ < 1024)
		printf("\t%5.1f B", maxUsed_/1.);
	else if(maxUsed_ < 1024*1024)
		printf("\t%5.1f KB", maxUsed_/1024.);
	else if(maxUsed_ < 1024*1024*1024)
		printf("\t%5.1f MB", maxUsed_/1024./1024.);
	else
		printf("\t%5.1f GB", maxUsed_/1024./1024./1024.);

	printf(" / ");

	if(size_ < 1024)
		printf("\t%5.1f B", size_/1.);
	else if(size_ < 1024*1024)
		printf("\t%5.1f KB", size_/1024.);
	else if(size_ < 1024*1024*1024)
		printf("\t%5.1f MB", size_/1024./1024.);
	else
		printf("\t%5.1f GB", size_/1024./1024./1024.);

	if(used_)
		printf(", \t%d still in use", used_);

	printf("\n");
}
