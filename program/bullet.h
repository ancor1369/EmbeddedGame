 
// Initialize the bullet. Don't have to do much here.
// Keep a pointer to this bullet's tank.
// Set the speed, and default the bullet to not in_flight.
Bullet::Bullet(Tank* t) {
    tank = t;
    //speed = ???;
    in_flight = false;
}
 
// If in_flight, do nothing. Otherwise,
// set the in_flight flag, and initialize values needed for
// the trajectory calculations. (x0, y0), (vx0, vy0), time
// Hint: tank->barrel_end(...) is useful here.
void Bullet::shoot(void) {
    in_flight = true;
}
 
// If the bullet is in flight, calculate its new position
// after a time delta dt.
void Bullet::update_position(float dt) {
 
}
 
int Bullet::time_step(float dt) {
    // If the bullet hasn't hit anything, 
    // redraw the bullet at its new location. 
    // If it has hit something (obstacle, tank, edge of the screen), 
    // set the in_flight flag back to false, explode the nearby area,
    // and return one of the following codes.
    //
    // return codes:
    //      BULLET_NO_COLLISION: no collision
    //      BULLET_OFF_SCREEN:   off the side of the screen
    //      Otherwise, return the color you've hit in 16bpp format. 
    return BULLET_NO_COLLISION;
