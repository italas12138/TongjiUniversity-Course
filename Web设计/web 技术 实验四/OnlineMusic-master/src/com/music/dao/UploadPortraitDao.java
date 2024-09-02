package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;

import com.music.utils.DButils;

public class UploadPortraitDao {

	public String upload(String portraitUrl,String userName) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "update user set  portrait = ? where username = ?";
		int rows = qr.update(sql, portraitUrl,userName);
		return rows>0? "true" : "false";
	}
	
}
