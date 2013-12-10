from tools import shutdown

def main():
    shutdown.install_signal_handlers()

    try:
        # start sever in another process or thread
        shutdown.wait_until_shutdown()
    finally:
        # stop server in elegent way
    
if __name__ == '__main__':
    main()
    
