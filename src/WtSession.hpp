/********************************************************************************
 *   This file is part of LUGHOS.						*
 *										*
 *										*
 *  LUGHOS is free software: you can redistribute it and/or modify		*
 *  it under the terms of the GNU General Public License as published by	*
 *  the Free Software Foundation, either version 3 of the License, or		*
 *  (at your option) any later version.						*
 *										*
 *  LUGHOS is distributed in the hope that it will be useful,			*
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of		*
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*
 *  GNU General Public License for more details.				*
 *										*
 *  We sincerily invite everybody to participate in LUGHO'S development. 	*
 *  Together we can achieve more.						*
 *										*
 *  (c) 2014, 2015 Jörn Mankiewicz (joern.mankiewicz@gmx.de), 			*
 *                 Irina Brodski (irina@brodski.de)				*
 *										*
 ********************************************************************************/

#ifndef WT_SESSION_HPP
#define WT_SESSION_HPP
#include <Wt/Auth/Login>
#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>
#include "WtUser.hpp"

namespace dbo = Wt::Dbo;
typedef Wt::Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;
class Session : public dbo::Session
{
public:
static void configureAuth();
Session(const std::string& sqliteDb);
~Session();
dbo::ptr<User> user() const;
Wt::Auth::AbstractUserDatabase& users();
Wt::Auth::Login& login() { return login_; }
static const Wt::Auth::AuthService& auth();
static const Wt::Auth::PasswordService& passwordAuth();
static const std::vector<const Wt::Auth::OAuthService *>& oAuth();
private:
dbo::backend::Sqlite3 connection_;
UserDatabase *users_;
Wt::Auth::Login login_;
};
#endif