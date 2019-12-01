
// sx is the x-coord of the bottom left corner of the tank
// sy is the y-coord of the same corner
// width is the width of the tank
// height is the height of the tank
Tank::Tank(int sx, int sy, int width, int height, int color) {
    x = sx; y = sy;
    w = width; h = height;
    tank_color = color;
    barrel_theta = PI/4.0;
    barrel_length = w;
    wheel_rad = 2.0;
    draw();
}
 
// Return the minimum x-coord of your tank's bounding box.
int Tank::min_x(void) { 
    return 0;
}    
 
// Return the minimum y-coord of your tank's bounding box.
int Tank::min_y(void) {
    return 0;
}
 
// Return the maximum x-coord of your tank's bounding box.
int Tank::max_x(void) { 
    return 0;
}
 
// Return the maximum y-coord of your tank's bounding box.
int Tank::max_y(void) {  
    return 1;
}
 
void Tank::barrel_end(int* bx, int* by) {
    // Set the x and y coords of the end of the barrel.
    // *bx = ???
    // *by = ???
}
 
void Tank::reposition(int dx, int dy, float dtheta) {
    // Blank out the old tank position, and
    //      Move the tank dx pixels in the x direction.
    //      Move the tank dy pixels in the y direction.
    //      Move the tank barrel by an angle dtheta. Don't allow it to go below parallel.
    
    // Do collision detection to prevent the tank from hitting things. 
    // (obstacles, side of the screen, other tanks, etc.)
}
 
 
// Example tank draw function. We expect you to get creative on this one!
void Tank::draw() {
    sync.line(x + w/2.0, y+h+wheel_rad, x + w/2.0 + barrel_length*cos(barrel_theta), y+h+wheel_rad + barrel_length*sin(barrel_theta), BLACK);
    sync.filled_rectangle(x, y+wheel_rad, x+w, y+h+wheel_rad, tank_color);
    sync.filled_circle(x+wheel_rad, y+wheel_rad, wheel_rad, BLACK);
    sync.filled_circle(x+w-wheel_rad, y+wheel_rad, wheel_rad, BLACK);
