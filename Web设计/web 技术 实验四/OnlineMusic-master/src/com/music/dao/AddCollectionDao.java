package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;

import com.music.utils.DButils;

public class AddCollectionDao {

	public String addCollection(String uid, String sid,String method) throws NumberFormatException, SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String flag = null;
		if (method.equals("add")) {
			String sql = "insert into collection values(null,?,?)";
			int rows = qr.update(sql, sid, Integer.parseInt(uid));
			flag = rows>0? "true" : "false";
		} else if (method.equals("delete")) {
			String sql = "delete from collection where sid = ? and uid = ?";
			int rows = qr.update(sql, sid, Integer.parseInt(uid));
			flag = rows>0? "true" : "false";
		}
		return flag;
	}

}
