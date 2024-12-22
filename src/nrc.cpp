#include <windows.h>
#include <EBApplication.hpp>
#include <EBFile.hpp>
#include <EBDirectory.hpp>
#include <EBMap.hpp>
#include <EBIniFile.hpp>

using namespace EBCpp;

class NRC : public EBObject<NRC>
{
public:
    NRC()
    {
        readConfig("config.ini");

        for( auto server : servers )
        {
            for( auto user : users )
            {
                EB_LOG_INFO("Start for " << server << " by user " << user.first);
                mount(drive, server, user.first, user.second);
                readDir(drive + ":\\", user.first, 0);
                unmount(drive);
            }
        }

        output();

        system("output.html");

        EBEventLoop::getInstance()->exit();
    }

    void output()
    {
        EBFile out;
        out.setFileName("output.html");
        if( out.open(EBFile::WRITE_ONLY) )
        {
            out.write("<html><head><title>Output</title><link rel='stylesheet' href='style.css'></head><body><table>");
            out.write("<tr><th>Directory</th>");
            for( auto user : users)
            {
                out.write("<th class='top_header'>");
                out.write(user.first);
                out.write("</th>");
            }
            out.write("</tr>");

            for( auto accessRight : accessRights )
            {
                out.write("<tr><th>");
                out.write(accessRight.first);
                out.write("</th>");

                for( auto user : users)
                {
                    out.write("<td class='");
                    EBString o = "?";
                    try
                    {
                        EBString right = accessRight.second->at(user.first);

                        if( right == "ACCESS_DENIED")
                        {
                            o = "X";
                        }
                        else if( right == "READ_ONLY" )
                        {
                            o = "R";
                        }
                        else if(right == "WRITE")
                        {
                            o = "W";
                        }
                        out.write(right);
                    }
                    catch(...)
                    {
                        out.write("NOT_FOUND");
                        // Do Nothing
                    }
                    out.write("'>");
                    out.write(o);
                    out.write("</td>");
                }

                out.write("</tr>");
            }
            out.write("</table></body></html>");
            out.close();
        }
    }

    void readConfig(const EBString& configFileName)
    {
        EB_LOG_INFO("Reading config.ini");

        EBIniFile config(configFileName);
        drive = config.getValue("general", "drive");
        maxLevel = config.getValue("general", "level").toInt();
        EB_LOG_INFO("Using Drive Letter: " << drive);

        for(auto server : config.getSection("servers"))
        {
            servers.append(server.second);
        }

        for(auto user : config.getSection("users"))
        {
            users.insert(user);
        }
    }

    bool mount(const EBString& driveName, const EBString& server, const EBString& username, const EBString& password)
    {
        int exitCode = system(EBString(EBString("net use ") + driveName + ": " + server + " /persistent:no /user:" + username + " " + password).dataPtr());
        return exitCode == 0;
    }

    bool unmount(const EBString& driveName)
    {
        EB_LOG_INFO("Try to unmount network directory!");
        int extiCode = system(EBString(EBString("net use ") + driveName + ": /d /y").dataPtr());
        return extiCode == 0;
    }

    void readDir(const EBString& directory, const EBString& username, int level)
    {
        if( maxLevel <= level )
            return;

        EBString dirKey = directory;

        if( accessRights.find(dirKey) == accessRights.end())
        {
            EBMap<EBString, EBString>* m = new EBMap<EBString, EBString>();
            accessRights.insert(std::pair<EBString, EBMap<EBString, EBString>*>(dirKey, m));
        }

        auto dirList = EBDirectory::getDirectoryList(directory);

        if( dirList.getSize() > 0 )
        {

            EBFile file;
            file.setFileName(directory + "test.file" );
            if( file.open(EBFile::WRITE_ONLY) )
            {
                file.close();
                file.remove(directory + "test.file");

                EB_LOG_INFO("WRITE >> " << directory );
                accessRights.at(dirKey)->insert(std::pair<EBString, EBString>(username, "WRITE"));
            }
            else
            {
                EB_LOG_INFO("READ_ONLY >> " << directory );
                accessRights.at(dirKey)->insert(std::pair<EBString, EBString>(username, "READ_ONLY"));
            }

            for( auto dir : dirList )
            {
                if( dir.get() == "." || dir.get() == ".." )continue;
                
                if( EBDirectory::exists(directory + dir.get()) )
                {
                    readDir(directory + dir.get() + "\\", username, level+1);
                }
                else
                {
                    if( !EBFile::exists(directory + dir.get()) )
                    {
                        readDir(dirKey + dir.get() + "\\", username, level+1);
                    }
                    else
                    {
                        EBFile file;
                        file.setFileName(directory + dir.get());

                        if( file.open(EBFile::READ_ONLY) )
                        {
                            EB_LOG_INFO("FILE NO DIRECTORY >> " << directory + dir.get());
                            file.close();
                        }
                        else
                        {
                            readDir(dirKey + dir.get() + "\\", username, level+1);
                        }
                    }
                }
            }
        }
        else
        {
            EB_LOG_INFO("ACCESS_DENIED >> " << directory);
            accessRights.at(dirKey)->insert(std::pair<EBString, EBString>(username, "ACCESS_DENIED"));
        }
    }

    //    DIR                     USER      RIGHT
    EBMap<EBString, EBMap< EBString, EBString >* > accessRights;
    EBString drive;
    int maxLevel;

    EBList<EBString> servers;
    EBMap<EBString, EBString> users;
};

EB_APPLICATION(NRC);