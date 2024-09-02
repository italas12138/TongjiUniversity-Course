package com.music.dao;

import java.sql.SQLException;
import java.util.Map;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.BeanHandler;
import org.apache.commons.dbutils.handlers.MapHandler;

import com.music.domain.User;
import com.music.utils.DButils;

public class GetUserInfoDao {

	public Map<String, Object> getiInfo(String userName) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "select * from user where username=?";
		Map<String, Object> map = qr.query(sql, new MapHandler(), userName);
		return map;
	}

}
