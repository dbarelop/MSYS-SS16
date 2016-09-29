from time import clock

def print_status(status):
    print("\033[3;3HStatus: %s" % status + 20 * " ")

def print_received(received):
    print("\033[5;7HReceived: %s" % received + 20 * " ")

def print_sending(sending):
    print("\033[6;7HSending: %s" % sending + 20 * " ")

def print_position(position):
    print("\033[7;7HPosition: %s" % position + 20 * " ")

def print_sollv(sollv):
    print("\033[8;7HLast SollV: %s" % sollv + 20 * " ")

def print_soll_lane(soll_lane):
    print("\033[9;7HLast Soll-Lane: %s" % soll_lane + 20 * " ")

def print_cur_time():
    print("\033[11;7HTime: %ds" % clock())

def print_error(error):
    print("\033[13;3HLast error (at %ds): %s" % (clock(), error) + 20 * " ")

def shutdown():
    print("\033[2J")
