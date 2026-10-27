#ifndef INCLUDED_QBtFileTimes_h
#define INCLUDED_QBtFileTimes_h

#include <QString>
#include <QtGlobal>

/*------- class declaration:
-------------------------------------------------------------------
 * The timestamps of a file, directory or symlink, captured from one
 * path and stamped onto another. Reading an item bumps its access
 * time and filling a directory bumps its modification time, so a
 * copy reads the times first and applies them when it is complete.
 * Qt cannot stamp a directory, so this goes through the native API.
 * Birth (creation) time is kept where the platform supports it.
-------------------------------------------------------------------*/
class QBtFileTimes
{
public:
   // Seconds and nanoseconds since the Unix epoch.
   struct Stamp
   {
      qint64 seconds     = 0;
      qint64 nanoseconds = 0;
   };

   // With is_link the link itself is read or stamped, not its target.
   static QBtFileTimes read( const QString& path, bool is_link );
   void apply( const QString& path, bool is_link ) const;

   bool valid() const { return valid_; }

private:
   bool  valid_     = false;
   bool  has_birth_ = false;
   Stamp access_;
   Stamp modification_;
   Stamp birth_;
};

#endif // INCLUDED_QBtFileTimes_h
