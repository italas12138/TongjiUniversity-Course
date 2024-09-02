package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;

import com.music.utils.DButils;

public class UpdateCommentPortraitdao {

	public String updateCommentPortrait(String userName, String imgurl){
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "update songComment set portrait=? where username=?";
		int rows=0;
		try {
			rows = qr.update(sql, imgurl,userName);
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return rows>0? "true" : "false";
	}

}
