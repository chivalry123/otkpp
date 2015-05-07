# Introduction #

OTK++ implements Qt4-based graphical user interface for demonstration purposes. All algorithms implemented in OTK++ are available via the GUI. However, it can only handle functions of two variables.

<img src='http://otkpp.googlecode.com/svn/site/images/Qt4-GUI.png'>

<h1>Quick reference</h1>

<h2>Problem setup and constraints <code>[1]</code></h2>

The user can either specify a symbolic expression or a predefined test function. A symbolic function expression may contain basic arithmetic operations and elementary functions supported by libmatheval. The list of predefined test functions contains test problems given by More, Garbow and Hillström(1981). The expressions of these function consist of sums of squared terms, and the number of summation terms is given with the m-parameter. When you have finished choosing the objective function, click <b>Update</b> to apply the changes.<br>
<br>
Rectangular constraints for the problem can be specified by clicking the <b>Pick</b> button and dragging on the plotting area <code>[6]</code>. They can also be entered manually to the text fields. Press <b>Update</b> to apply the entered values. <b>Deactivate</b> clears previously specified constraints.<br>
<br>
<h2>Solver setup and starting point <code>[2]</code></h2>

The starting point can be picked from the plotting area after activating the picking tool by pressing <b>Pick</b>. Select the algorithm to use from the combo box.<br>
<br>
<b>Note</b>: Specifying the stopping criterion does not work with the current version.<br>
<br>
<h2>Results summary <code>[3]</code></h2>

A summary of the results of the iteration is shown here. If several iteration plots are drawn, you can pick them from the plotting area <code>[6]</code> after pressing <b>Pick</b>. <b>Deselect</b> clears any selections, and <b>Remove selected</b> removes the selected iteration plot from the plotting area.<br>
<br>
<h2>Picking iterates and search lines <code>[4]</code></h2>

Iterates and search lines can be picked from the iteration plot. Pressing the <b>Pick</b> button activates the picking tool. Only one iterate or search line can be picked at a time. The <b>Deselect</b> buttons clear any selected iterates or search lines. The coordinates of the selected iterate and the corresponding function value and gradient norm are shown. For a selected search line, the objective function value along the search direction is also plotted as a function of step length.<br>
<br>
<h2>Plotting utilities <code>[5]</code></h2>

Plotting ranges can be entered manually in the text boxes and clicking the <b>Update</b> button. The plotting ranges can also be adjusted with zooming and panning tools: try the <b>zoom out</b>, <b>zoom in</b> and <b>pan</b> buttons. <b>Clear</b> removes any iterates or selections from the plot. Plots can also be saved in pdf format by clicking the <b>Save...</b> button.<br>
<br>
<h2>Plotting area <code>[6]</code></h2>

If <b>contour</b> is selected from the plot type button, this widget draws the contour plot of the objective function and iteration paths drawn on top of them. If the plotting type is <b>surface</b>, this widget shows a 3d surface of the objective function.<br>
<br>
<b>Note:</b> The 3d plotting widget requires OpenGL support.