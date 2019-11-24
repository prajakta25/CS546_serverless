import os

def control():
    '''print("tell me to stop by pressing enter")'''
    stopping = raw_input("Press enter to stop")
    with open("stop",'w') as stop_file:
        stop_file.write("1")
    os.system(" python printTable.py")
    
def execution():
    with open("stop",'w') as stop_file:
        stop_file.write("0")
    os.system("bash comm_exec.sh &")
    '''os.system(" python printTable.py")'''
def main():
    
    pid = os.fork()
    if pid > 0:
        control()
    else:
        execution()
if __name__ == "__main__":
    main()
