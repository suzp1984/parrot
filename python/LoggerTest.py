def initlog():
	import logging

	logger = logging.getLogger()
	hdlr = logging.FileHandler('log.test')
	formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
	hdlr.setFormatter(formatter)
	logger.addHandler(hdlr)
	logger.setLevel(logging.NOTSET)

	return logger


logger = initlog()

logger.info("info test")
logger.error("error")
