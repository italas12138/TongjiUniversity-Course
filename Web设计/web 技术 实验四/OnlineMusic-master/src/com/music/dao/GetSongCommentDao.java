package com.music.dao;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.dbutils.handlers.MapListHandler;

import com.music.domain.SongComment;
import com.music.utils.DButils;

public class GetSongCommentDao {

	public List<Map<String, Object>> getComment(String sid,String uid,String method) throws SQLException {
		QueryRunner qr = new QueryRunner(DButils.getDataSource());
		List<Map<String, Object>> list = null;
		if (method.equals("songComment")) {
			String sql = "select * from songComment where sid=?";
			list = qr.query(sql, new MapListHandler(), sid);
		} else if (method.equals("myComment")) {
			String sql = "select * from songComment where uid=?";
			list = qr.query(sql, new MapListHandler(), uid);
		}
		return list;
	}

}
