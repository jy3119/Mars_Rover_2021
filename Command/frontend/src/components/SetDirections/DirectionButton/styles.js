import { makeStyles } from '@material-ui/core/styles';

export default makeStyles((theme) => ({
    root: {
      '& .MuiTextField-root': {
        margin: theme.spacing(1),
      },
    },
    paper: {
      padding: theme.spacing(2),
    },
    form: {
      display: 'flex',
      flexWrap: 'wrap',
      justifyContent: 'center',
    },
    full_width_button: {
      margin: 10,
    },
    direction_button: {
      display: 'flex',
        width: 100,
        margin: 10
    }
  }));

  