#include "TLogger/inc/TLogger.h"

using namespace TLogger;

unsigned LoggerFacade::ref_count = 0;
std::unique_ptr<LoggerFacade::LoggerFacadeInstance> LoggerFacade::logger_facade_inst_priv;
std::unique_ptr<LoggerFacade> LoggerFacade::logger_facade_inst;
bool LoggerFacade::instance_created;