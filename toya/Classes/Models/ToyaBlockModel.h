//
//  BlockModel.h
//
//
//  Created by 6Sum Studio on 2/27/16.
//
//

#ifndef __TOYA_BLOCK_MODEL_H__
#define __TOYA_BLOCK_MODEL_H__

class BlockModel : public BoxObstacle {  // Block demo game controller line 420
private:
    /** The size of this block. **/
    Size _size;
    /** The texture of this block. **/
    String _texture;
    /** The coordinate of this block. **/
    Vec2 _pos;
    
#pragma mark -
#pragma mark Static Constructors
    /**
     * Creates a new block at the origin.
     *
     * The block is 1 unit by 1 unit in size. The block is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  An autoreleased physics object
     */
    static BlockModel* create();
    
    /**
     * Creates a new block with the given position
     *
     * The block is 1 unit by 1 unit in size. The block is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  An autoreleased physics object
     */
    static BlockModel* create(const Vec2& pos);
    
    /**
     * Creates a new block with the given position and size.
     *
     * The block size is specified in world coordinates.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as throe physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  size       The dimensions of the box.
     *
     * @return  An autoreleased physics object
     */
    static BlockModel* create(const Vec2& pos, const Size& size);
    
    /**
     * Creates a new block with the given position and size.
     *
     * The block size is specified in world coordinates.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as throe physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos      Initial position in world coordinates
     * @param  size       The dimensions of the box.
     * @param  texture    The texture
     *
     * @return  An autoreleased physics object
     */
    static BlockModel* create(const Vec2& pos, const Size& size, const String& texture);
    
#pragma mark -
#pragma mark Initializers
CC_CONSTRUCTOR_ACCESS:
    /*
     * Creates a new block at the origin.
     */
    BlockModel(void) : BoxObstacle() { }
    
    /**
     * Destroys this block, releasing all resources.
     */
    virtual ~BlockModel(void);
    
    /**
     * Initializes a new block at the origin.
     *
     * The block is 1 unit by 1 unit in size. The block is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init() override { return init(Vec2::ZERO, Size::ZERO); }
    
    /**
     * Initializes a new block with the given position
     *
     * The block is 1 unit by 1 unit in size. The block is scaled so that
     * 1 pixel = 1 Box2d unit
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos  Initial position in world coordinates
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2& pos) override { return init(pos, Size(1,1)); }
    
    /**
     * Initializes a new block with the given position and size.
     *
     * The block size is specified in world coordinates.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos   Initial position in world coordinates
     * @param  size   The dimensions of the block.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2& pos, const Size& size) override;
    
    /**
     * Initializes a new block with the given position and size.
     *
     * The block size is specified in world coordinates.
     *
     * The scene graph is completely decoupled from the physics system.
     * The node does not have to be the same size as the physics body. We
     * only guarantee that the scene graph node is positioned correctly
     * according to the drawing scale.
     *
     * @param  pos   Initial position in world coordinates
     * @param  size   The dimensions of the block.
     * @param  texture The texture of the block.
     *
     * @return  true if the obstacle is initialized properly, false otherwise.
     */
    virtual bool init(const Vec2& pos, const Size& size, const String& texture) override;
    
    /**
     * Returns the position of the block.
     *
     * @return the position of the block.
     */
    virtual const Vec2& getPosition() const { return _pos; }
    
    /**
     * Sets the position of the block.
     *
     * @param value  the postion assigned to the block.
     */
    virtual void setPosition(const Vec2& value) { _pos.set(value); }
    
    /**
     * Returns the texture (key) for this block.
     *
     * The value returned is not a Texture2D value.  Instead, it is a key for
     * accessing the texture from the asset manager.
     *
     * @return the texture (key) for this rocket
     */
    const string& getTexture() const { return _texture; }
    
    /**
     * Set the texture of this block.
     *
     * @param  texture    the texture (key) for this block
     */
    virtual void setTexture(string texture) { _texture = texture; }
    
    /**
     * Returns the size for this block.
     *
     * @return the size for this rocket
     */
    const string& getSize() const { return _size; }
    
    /**
     * Returns the size for this block.
     *
     * @param  size    the size for this block
     */
    virtual void setSize(const Size& size) { _size = size; }
};

#endif
/* defined(__TOYA_BLOCK_MODEL_H__) */


