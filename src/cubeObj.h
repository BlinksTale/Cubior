/*
 * cubeObj.h
 * by Brian Handy
 * 2/11/12
 * Header for any cube in the game (obstacles, items and players)
 */

#ifndef CUBEOBJ
#define CUBEOBJ

class CubeObj {
  protected:
    bool neighbors[6],visibleNeighbors[6],edges[6],toldToMove;
    float momentumX, momentumY, momentumZ, movementSpeed, movementDivision;
    int x, y, z, diffX, diffY, diffZ, oldX, oldY, oldZ,
        landedOnX, landedOnY, landedOnZ, landedOnCount;
    bool hasMaterial, playerStatus, cameraStatus, newJump, loseMomentumOnLock;
    bool locked, lockable, permalocked, jumpable, grounded, lastGrounded, doubleLastGrounded, neighborsSet, visibleNeighborsSet;
    int maxSpeed, friction;
    int maxJump, jumpSpeed, jumpSpeedRatio, gravity;
    int material;
    bool jumping, lastJumping, collision, lastCollision;
    CubeObj* landedOn;
    float landedOnDirectionDiff;
    float newFriction, strength, direction; // for use with new friction technique
    bool invisible;
  public:
    CubeObj();

    void tick();

    void calculateDiff();
    void fall();
    void land();
    void landOn(CubeObj*);
    bool updateLandedOnPos();
    int getLandedOnCount();
    void freeze();
    
    // return status
    bool isMoving();
    bool isMovingQuickly();
    bool isPlayer();
    bool isCamera();
    void setCameraStatus(bool);

    void jump(bool);

    virtual void collisionEffect(CubeObj*);

    void setLock(bool);
    bool getLock();
    void setPermalock(bool);
    bool getPermalock();
    bool getGrounded();
    bool getStillGrounded();
    bool getNotGrounded();
    bool getLanded();
    bool justJumped();
    bool justBumped();

    void setPos(int,int,int);
    void setX(int);
    void setY(int);
    void setZ(int);

    int distToCube(CubeObj*);
    
    void changePos(int,int,int);
    void changeX(int);
    void changeY(int);
    void changeZ(int);
    void changePosTowards(CubeObj*,double);
    void changePosTowards(int, int, int, double);
     
    void setMomentumX(int);
    void setMomentumY(int);
    void setMomentumZ(int);
    
    void movePos(int,int,int);
    void moveX(float);
    void moveY(float);
    void moveZ(float);

    int get(int);
    int getX();
    int getY();
    int getZ();
    int getAngleX();
    int getAngleY();
    int getAngleZ();
    int getMomentumX();
    int getMomentumY();
    int getMomentumZ();
    
    void setNeighbors(bool,bool,bool,bool,bool,bool);
    void setVisibleNeighbors(bool,bool,bool,bool,bool,bool);
    bool* getNeighbors();
    bool* getVisibleNeighbors();
    // These two check neighbor status and return if double neighbors in one or two dimensions respectively
    bool isColumn();
    bool isWall();
    bool isVertWall();
    
    void setEdges(bool,bool,bool,bool,bool,bool);
    bool* getEdges();
    void setCollision(bool);
    void applyCollisionMomentumX();
    void applyCollisionMomentumZ();
    bool getCollision();
    virtual int getWidth();
    virtual int getHeight();
    virtual int getSize(int);
    int getMaterial();
    void setMaterial(int);

    float getStrength();
    float getDirection();
    void setInvisible(bool b) { invisible = b; }
    bool isInvisible() { return invisible; }
};

#endif 
