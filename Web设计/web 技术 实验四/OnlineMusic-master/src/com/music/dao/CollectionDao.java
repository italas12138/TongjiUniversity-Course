package com.music.dao;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.BeanListHandler;
import org.apache.commons.dbutils.handlers.MapListHandler;

import com.music.domain.Collection;
import com.music.utils.DButils;
import com.sun.org.apache.bcel.internal.generic.NEW;

public class CollectionDao {

	public List<Map<String, Object>> getCollection(int uid) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		String sql = "select * from Collection where uid = ?";
		List<Map<String, Object>> list = qr.query(sql, new MapListHandler(),uid);
		return list;
	}

}
