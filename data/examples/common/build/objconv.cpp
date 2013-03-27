#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgDB/Registry>




int
main(int argc, char** argv)
{
    if( argc == 3 ) {
        {
            osgDB::ReaderWriter::Options* options = new osgDB::ReaderWriter::Options() ;
            options->setOptionString("WriteImageHint=IncludeData") ;
            osgDB::Registry::instance()->setOptions(options);
        }
        {
            osgDB::ReaderWriter::Options* options = new osgDB::ReaderWriter::Options() ;
            options->setOptionString("Compressor=zlib") ;
            osgDB::Registry::instance()->setOptions(options);
        }



        osg::ref_ptr<osg::Object>  obj = osgDB::readObjectFile(argv[1]) ;
        osgDB::writeObjectFile(*obj, argv[2]) ;
    }

    return 0 ;
}
