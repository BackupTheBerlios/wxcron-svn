/**
 * Name:        WXCConfig.h
 * Purpose:
 * Author:      Christian Buhtz
 * Modified by:
 * Created:     2008-05-28
 * Copyright:   (c) 2008 Christian Buhtz <exsudat@gmx.de>
 * Licence:     GNU General Public License (Version 3)
 ***
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/


#ifndef WXCCONFIG_H
#define WXCCONFIG_H

///
class WXCConfig
{
    private:
        ///
        static WXCConfig        sConfig_;

        /** Maximum size of the log-file in KiloBytes.
            Set it to '0' if the maximum size does not matter. */
        long                    lMaxSize_;

        /// ctor
        WXCConfig ();

    public:
        /// virtual dtor
        virtual ~WXCConfig ();

		/// need to be called first!
		void Init ();

        ///
        static WXCConfig& Instance ()
        {
            return sConfig_;
        }

        /// Read the data from the config file.
        void ReadConfig ();
        /// Save the data to the config file.
        bool SaveConfig ();

        ///
        long GetMaxLogFileSizeInKB ();
        ///
        void SetMaxLogFileSizeInKB (long lSize);
};

#endif    // WXCCONFIG_H
