package com.music.dao;

import java.sql.SQLException;

import org.apache.commons.dbutils.QueryRunner;

import com.music.utils.DButils;

public class FabulousDao {

	public boolean updateFabulous(String fabulousId, String praiseNum) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "update songComment set praiseNum = ? where fabulousId = ?";
		int rows = qr.update(sql,Integer.parseInt(praiseNum)+1,Integer.parseInt(fabulousId));
		return rows>0? true : false;
	}

}
