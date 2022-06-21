import matplotlib.pyplot as plt
import numpy as np

# The value of the deadload is 50 N 
deadload = 50
# The expected failure load
expected_failureload = 160
# The offset for text
offset = 10

def x_axis_generator(limit):
    """ x axis represents liveload in unit Newton
        the incremental step is specified by 10 N
        @param limit, the upper limit of the liveload
        @return list, from 0 up to limit
    """
    x_axis = []
    liveload = 0
    while liveload <= limit:
        x_axis.append(liveload)
        liveload += 20
    return x_axis

def y_axis_generator(param_deadload, param_liveload, liveload):
    """ y axis represents force in the member of a truss
        @param, param_deadload, param_liveload, liveload the coeffcient of 
        deadload and liveload, a list containing all liveload
        @return list, the corresponding force based on the value of deadload,
        liveload
    """
    y_axis = []
    for i in liveload:
        each_force = param_deadload * deadload + param_liveload * i
        y_axis.append(each_force)
    return y_axis

def position_solver(param_deadload, param_liveload, liveload):
    """ calculate the position when the truss member fails
        @return list, containing x and y position
    """
    x_position = (expected_failureload - param_deadload * deadload) / param_liveload
    position = [x_position, expected_failureload]
    return position
    
def graph_generator(x_axis, y_axis, member, limit, position):
    """ generate the graph in terms of x and y axis
        @param x_axis, y_axis, member, limit, the date point needed to plot the diagram
        the name of the member that to be plotted
        @return plt object
    """
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.plot(x_axis, y_axis, 'k')
    ax.text(position[0], position[1] - offset, "(" + str(round(position[0], 2)) + "," + str(round(position[1], 2)) + ')', fontsize = 12)
    plt.hlines(160, 0, limit, linestyles = "dashed", colors = 'r')
    ax.set(title = "Member " + member + ": Internal force VS Live load Diagram",
           xlabel = "Live load in (N)",      
           ylabel = "Internal force in member " + member + " (N)")
    plt.grid()
    ax.set_xticks(x_axis)
    ax.set_yticks(y_axis)
    return plt

def __main__():
    """ control the flow """
    member = input("WHich truss member are you currently interested in? ")
    limit = int(input("Please tell me what is the upper limit of the liveload: "))
    x_axis = x_axis_generator(limit)
    co_1 = float(input("Please tell me the coefficient for deadload: "))
    co_2 = float(input("Please tell me the coefficient for liveload: "))
    position = position_solver(co_1, co_2, x_axis)
    y_axis = y_axis_generator(co_1, co_2, x_axis)
    graph_generator(x_axis, y_axis, member, limit, position)
    plt.show()
    
__main__()
