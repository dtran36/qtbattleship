#ifndef SHIP_H
#define SHIP_H

enum ShipType{empty,carrier,battleship,submarine,destroyer,patrol};

class Ship
{
public:
    /**
     * @brief Ship Constructs ship with chosen ShipType.
     * @param t possible types defined by ShipType enum
     */
    Ship(ShipType t);

    /**
     * @brief getType Gets object's ship type.
     * @return ShipType
     */
    ShipType getType() const;

    /**
     * @brief Returns X coordinate of chosen square.
     * @param index square's index (0-4)
     * @return square's Y coordinate
     */
    int x(int index) const;

    /**
     * @brief Returns Y coordinate of chosen square.
     * @param index square's index (0-4)
     * @return square's X coordinate
     */
    int y(int index) const;

    /**
     * @brief setVertical Sets current object's squares so that ship is vertical.
     */
    void setVertical();

    /**
     * @brief setHorizontal Sets current object's squares so that ship is horizontal.
     */
    void setHorizontal();

    /**
     * @brief rotate If ship is vertical, calls setHorizontal, vice versa.
     */
    void rotate();

private:
    /**
     * @brief setX Changes the X value of a ship square.
     * @param index choose square to change 0,1,2,3,4
     * @param x value to set chosen square's X
     */
    void setX(int index, int x);

    /**
     * @brief setY Changes the y value of a ship square.
     * @param index choose square to change 0,1,2,3,4
     * @param y value to set chosen square's Y
     */
    void setY(int index, int y);

    ShipType type; //!< type of ship possible ships are defined in ShipType enum

    int coords[5][2]; //!< 2d array holding x,y coordinates for 5 squares
    bool vertical; //!< true if ship is in vertical position, false if horizontal
};

#endif // SHIP_H
