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