#ifndef Converter_h
#define Converter_h
#include <stdio.h>
#include <string.h>
#include <string>

class Str {
    public:

        Str()
         : m_buf(NULL)
        {
        }

        Str(const char* s)
        {
            if(!s) {
                m_buf = NULL;
            } else {
                int len = strlen(s) + 1;
                m_buf = new char[len];
                strcpy(m_buf, s);
            }

        }

        ~Str()
        {
            if(m_buf) {
                delete [] m_buf;
                m_buf = NULL;
            }
        }

        Str(const Str& s)
        {
            if (!s.m_buf) {
                m_buf = NULL;
            } else {
                m_buf = new char[strlen(s.m_buf) + 1];
                strcpy(m_buf, s.m_buf);
            }
        }

        Str& operator = (const Str& s)
        {
            if(this == &s)
                return *this;

            if(!s.m_buf) {
                m_buf = NULL;
            } else {

                if (m_buf) {
                    delete [] m_buf;
                    m_buf = NULL;
                }

                m_buf = new char[strlen(s.m_buf) + 1];
                strcpy(m_buf, s.m_buf);
            }

            return *this;
        }

        char* c_str() const
        {
            return m_buf;
        }

    private:
        char *m_buf;
};

class Converter
{
public:
   Converter()
   {
   }

   virtual ~Converter()
   {
   }

   Str& buffer()
   {
       return m_buffer;
   }

   Converter& write(const char *text, unsigned int length)
   {
      Str s(text); 
      m_buffer = s;
      return *this;
   }

   Converter& write(const char *text)
   {
      return write(text, strlen(text));
   }

   Converter& operator <<(bool v)
   {
      const char *out = v ? "true" : "false";
      return write(out);
   }

   Converter& operator <<(short v)
   {
      char buffer[64];
      sprintf(buffer, "%hd", v);
      return write(buffer);
   }

   Converter& operator <<(unsigned short v)
   {
      char buffer[64];
      sprintf(buffer, "%hu", v);
      return write(buffer);
   }

   Converter& operator <<(int v)
   {
      char buffer[64];
      sprintf(buffer, "%d", v);
      return write(buffer);
   }

   Converter& operator <<(unsigned int v)
   {
      char buffer[64];
      sprintf(buffer, "%u", v);
      return write(buffer);
   }

   Converter& operator <<(long v)
   {
      char buffer[64];
      sprintf(buffer, "%ld", v);
      return write(buffer);
   }

   Converter& operator <<(unsigned long v)
   {
      char buffer[64];
      sprintf(buffer, "%lu", v);
      return write(buffer);
   }

   Converter& operator <<(float v)
   {
      char buffer[128];
      sprintf(buffer, "%.16g", double(v));
      return write(buffer);
   }

   Converter& operator <<(double v)
   {
      char buffer[128];
      sprintf(buffer, "%.16g", v);
      return write(buffer);
   }

   Converter& operator <<(long double v)
   {
      char buffer[128];
      sprintf(buffer, "%.16g", double(v));
      return write(buffer);
   }

   Converter& operator <<(const void *v)
   {
      char buffer[64];
      sprintf(buffer, "%p", v);
      return write(buffer);
   }

   Converter& operator <<(const char *v)
   {
      return write(v ? v : "NULL");
   }

   Converter& operator <<(char c)
   {
      char buffer[16];
      sprintf(buffer, "%c", c);
      return write(buffer);
   }

   Converter& operator <<(const std::string &s)
   {
      return write(s.c_str(), s.length());
   }


private:
   Str m_buffer;
};

#endif
