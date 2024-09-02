package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.BeanHandler;

import com.music.domain.User;
import com.music.utils.DButils;

public class RegisterDao {

	public String register(User user) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "select * from user where username=?";
		User user1 = qr.query(sql, new BeanHandler<User>(User.class),user.getUsername());
		System.out.println(user1);
		if (user1 == null) { // 用户名不存在
			String sql1 = "insert into user values(null,?,?,?,?,?,?,?)";
			int rows = qr.update(sql1,user.getUsername(),user.getPassword(),user.getTelephone(),user.getQq(),user.getEmail(),user.getRegisterDate(),user.getPortrait());
			return rows>0? "注册成功" : "注册失败，请重试！";
		} else { // 用户名已存在
			return "此用户名已经被注册，请更换用户名！";
		}
		
		
	}

}
