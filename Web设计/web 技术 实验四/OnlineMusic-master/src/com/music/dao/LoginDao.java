package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.BeanHandler;

import com.music.domain.User;
import com.music.utils.DButils;

public class LoginDao {

	public User login(String userName, String passWord) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "select * from user where username=? and password=?";
		User user= qr.query(sql, new BeanHandler<User>(User.class), userName,passWord);
		return user;
	}

}
