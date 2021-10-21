/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajimenez <ajimenez@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/17 16:13:34 by ajimenez          #+#    #+#             */
/*   Updated: 2021/10/21 19:22:53 by ajimenez         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	ft_open_error(char *s, t_pipex *ps)
{
	ft_putstr_fd(s, 1);
	exit(1);
}

char	*path_cmd(t_pipex *ps, char **envp, char *cmd)
{
	ssize_t	i;
	char	*path;
	char	*tmp;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp("PATH=", envp[i], 5) == 0)
			ps->paths = ft_split(ft_strchr(envp[i], '/'), ':');
		i++;
	}
	i = 0;
	while (ps->paths[i])
	{
		tmp = ft_strjoin(ps->paths[i], "/");
		path = ft_strjoin(tmp, ft_split(cmd, ' ')[0]);
		free(tmp);
		if (access(path, R_OK) == 0)
			return (path);
		i++;
	}
	if (!path)
	{
		ft_putstr_fd(ft_strjoin(cmd, ": command not found"), 1);
//		free(ps);
	}
	return (0);
}

void	exec_cmd(t_pipex *ps, char *env[])
{
	char	*path;
	char	**cmd;

	path = path_cmd(ps, env, ps->cmd_1);
	cmd = ft_split(ps->cmd_1, ' ');
	execve(path, cmd, env);
}

/* ************************************************************************** */
/*  STDOUT_FILENO = 1                                                         */
/*  STDIN_FILENO  = 0                                                         */
/*  WRITE_END     = 1                                                         */
/*  READ_END      = 0                                                         */
/* ************************************************************************** */


void	ft_exec_cmd1(t_pipex *ps, char **env, int *fd)
{
	char	*path;
	char	**cmd;


	close(fd[READ_END]);
	ps->fd_1 = open(ps->infile, O_RDONLY);/*abrir archivo 1*/
	if (ps->fd_1 == -1)
		ft_open_error("error ", ps);
	path = path_cmd(ps, env, ps->cmd_1);/*recoger comandos*/
	cmd = ft_split(ps->cmd_1, ' ');/*split del comando por si hay argumentos*/
	dup2(ps->fd_1, STDIN_FILENO);
	close(ps->fd_1);
	dup2(fd[WRITE_END], STDOUT_FILENO);/**/
	close(fd[WRITE_END]);
	execve(path, cmd, env);
}

void	ft_exec_cmd2(t_pipex *ps, char **env, int *fd)
{
	char	*path;
	char	**cmd;

	close(fd[READ_END]);
	ps->fd_2 = open(ps->infile, O_RDONLY);
	if (ps->fd_2 == -1)
		ft_open_error("error ", ps);
	path = path_cmd(ps, env, ps->cmd_2);
	cmd = ft_split(ps->cmd_2, ' ');
	dup2(fd[READ_END], STDOUT_FILENO);
	close(fd[READ_END]);
	dup2(ps->fd_2, STDOUT_FILENO);
	close(ps->fd_2);
	execve(path, cmd, env);
}
