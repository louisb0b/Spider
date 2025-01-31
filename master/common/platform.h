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

#ifndef PLATFORM_H
#define PLATFORM_H

#include <type.h>
#include <Message.h>

class LRT;
class LrtCommunicator;
class SpiderCommunicator;

class Platform{
public:
	/** File Handling */
	virtual int fopen(const char* name) = 0;
	virtual void fprintf(int id, const char* fmt, ...) = 0;
	virtual void fclose(int id) = 0;

	/** Memory Handling */
	virtual void* virt_to_phy(void* address) = 0;
	virtual int getMinAllocSize() = 0;
	virtual int getCacheLineSize() = 0;

	/** Time Handling */
	virtual void rstTime(ClearTimeMsg* msg) = 0;
	virtual void rstTime() = 0;
	virtual Time getTime() = 0;

	/** Platform getter/setter */
	static inline LRT* getLrt();
	static inline Platform* get();
	static inline LrtCommunicator* getLrtCommunicator();
	static inline SpiderCommunicator* getSpiderCommunicator();

protected:
	Platform();
	virtual ~Platform();

	static LRT* lrt_;
	static Platform* platform_;
	static LrtCommunicator* lrtCom_;
	static SpiderCommunicator* spiderCom_;
};


inline Platform* Platform::get(){
	if(platform_)
		return platform_;
	else
		throw "Error undefined platform\n";
}

inline LRT* Platform::getLrt(){
	if(lrt_)
		return lrt_;
	else
		throw "Error undefined LRT\n";
}

inline LrtCommunicator* Platform::getLrtCommunicator(){
	if(lrtCom_)
		return lrtCom_;
	else
		throw "Error undefined LRT Communicator\n";
}

inline SpiderCommunicator* Platform::getSpiderCommunicator(){
	if(spiderCom_)
		return spiderCom_;
	else
		throw "Error undefined Spider Communicator\n";
}

#endif/*PLATFORM_H*/
