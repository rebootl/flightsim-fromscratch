
class HeliManipulator : public StandardManipulator {
    //  third-person-view following a position, orientation & zoom by mouse, always up

    // --> what is this ?
    //typedef StandardManipulator inherited;

    public:
        // constructor
        // evtl. give it the node to follow ??
        // default orient./zoom etc.
        HeliManipulator( osg::Vec3d pos );

        // --> needed ?
        //virtual osg::Matrixd getMatrix() const;

        // THIS is called by the viewer to create the camera view !!!
        virtual osg::Matrixd getInverseMatrix() const;

        // position the camera is fixed to, e.g. aircraft position
        virtual void setPosition( const osg::Vec3d& pos );
        // evtl. use offset later
        //virtual void setCenter( const osg::Vec3d& center );
        //virtual void setOffset( const osg::Vec3d& off );
        virtual void setRotation( const osg::Quat& rot );
        virtual void setDistance( double dist );

    protected:
        // --> MAKE AN INPUT HANDLER FOR THIS
        //virtual bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
        //...

        osg::Vec3d _center;
        osg::Quat  _rotation;
        double     _distance;
};

HeliManipulator::Helimanipulator( osg::Vec3d pos ) {

    // set StandardManipulator settings
    setVerticalAxisFixed(true);
    setAllowThrow(false);

    _center = pos;
    // -->
    //_rotation =
    _distance = 1.;

}

// (from OrbitManipulator)
/** Get the position of the manipulator as 4x4 matrix.*/
osg::Matrixd OrbitManipulator::getMatrix() const
{
    return osg::Matrixd::translate( 0., 0., _distance ) *
           osg::Matrixd::rotate( _rotation ) *
           osg::Matrixd::translate( _center );
}
/** Get the position of the manipulator as a inverse matrix of t
he manipulator,
    typically used as a model view matrix.*/
osg::Matrixd OrbitManipulator::getInverseMatrix() const
{
    return osg::Matrixd::translate( -_center ) *
           osg::Matrixd::rotate( _rotation.inverse() ) *
           osg::Matrixd::translate( 0.0, 0.0, -_distance );
}

