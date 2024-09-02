package com.music.service;

import java.sql.SQLException;

import com.music.dao.LoginDao;
import com.music.domain.User;

public class LoginService {

	public User login(String userName, String passWord) throws SQLException {
		LoginDao dao = new LoginDao();
		User user = dao.login(userName,passWord);
		return user;
	}

}
