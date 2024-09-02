package com.music.service;

import java.sql.SQLException;

import com.music.dao.RegisterDao;
import com.music.domain.User;

public class RegisterService {

	public String register(User user) throws SQLException {
		RegisterDao dao = new RegisterDao();
		String regMsg = dao.register(user);
		return regMsg;
	}

}
