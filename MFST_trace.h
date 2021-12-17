#pragma once

static int FST_TRACE_n = 0;
static char* rbuf = new char[205];
static char* lbuf = new char[205];
static char* sbuf = new char[1024];
static bool output;

#define MFST_TRACE_START if (0) {std::cout << std::endl	\
								<< std::setw( 4) << std::left << "Шаг" << ":"	\
								<< std::setw(20) << std::left << "Правило"		\
								<< std::setw(30) << std::left << "Входная лента"\
								<< std::setw(20) << std::left << "Стек"			\
								<< std::endl;}								



#define MFST_TRACE1	if (0) {\
						std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": "\
						<< std::setw(20) << std::left << rule.getCRule(rbuf, nrulechain)\
						<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position)\
						<< std::setw(20) << std::left << getCSt(sbuf)\
						<< std::endl;\
					}

#define MFST_TRACE2	if (0) {\
						std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": "\
						<< std::setw(20) << std::left << " "\
						<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position)\
						<< std::setw(20) << std::left << getCSt(sbuf)\
						<< std::endl;\
					}													

#define MFST_TRACE3	if (0) {\
						std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": "\
						<< std::setw(20) << std::left << " "\
						<< std::setw(30) << std::left << getCLenta(lbuf, lenta_position)\
						<< std::setw(20) << std::left << getCSt(sbuf)\
						<< std::endl;\
					}

#define MFST_TRACE4(c)			if (0) std::cout << std::setw(4) << std::left << ++ FST_TRACE_n << ": " << std::setw(20) << std::left << c << std::endl;

#define MFST_TRACE5(c)			if (0) std::cout << std::setw(4) << std::left <<    FST_TRACE_n << ": " << std::setw(20) << std::left << c <<std::endl;

#define MFST_TRACE6(c,k)		if (0) std::cout << std::setw(4) << std::left <<    FST_TRACE_n << ": " << std::setw(20) << std::left <<c << k <<  std::endl;

#define MFST_TRACE7	if (0) {\
						std::cout << std::setw(4) << std::left << state.lenta_position  << ": "\
						<< std::setw(20) << std::left << rule.getCRule(rbuf,state.nrulechain)\
						<< std::endl;\
					}